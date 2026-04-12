/*
 * ============================================================
 * Betaflight Firmware Configuration
 * Target:  SpeedyBee F405 AIO (SPEEDYBEEF405AIO)
 * Motors:  Happymodel EX1404 1404 KV4800 (x4)
 * ESC:     Built-in 35A BLHeli_32 4-in-1
 * BF Ver:  4.5.x
 * ============================================================
 *
 * USAGE:
 *   1. Flash SPEEDYBEEF405AIO target via Betaflight Configurator
 *   2. Build this file (or extract CLI commands from main())
 *   3. Alternatively, use the companion Python flasher script
 *      to send CLI commands automatically over USB serial.
 *
 * NOTE: This file models Betaflight's internal config structs
 *       and mirrors the actual BF source layout for clarity.
 *       It can also be used as a reference implementation or
 *       compiled as a standalone config validator/generator.
 * ============================================================
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

/* ============================================================
 * SECTION 1 — BOARD IDENTITY
 * ============================================================ */
#define BOARD_NAME          "SPEEDYBEEF405AIO"
#define MANUFACTURER_ID     "SPBE"
#define MCU                 "STM32F405"
#define FC_FIRMWARE_VERSION "4.5.0"

/* ============================================================
 * SECTION 2 — MOTOR / ESC CONFIGURATION
 * ============================================================ */

/* EX1404 motor spec: 12N14P stator → 12 magnetic poles */
#define MOTOR_POLE_COUNT        12

/*
 * DSHOT600 — optimal for BLHeli_32 built-in ESC.
 * Bidirectional DSHOT enables RPM telemetry for RPM filtering.
 *
 * Protocol options (in order of preference for this ESC):
 *   DSHOT600  ← recommended
 *   DSHOT300
 *   DSHOT150
 *   MULTISHOT (not recommended — analog)
 */
#define MOTOR_PWM_PROTOCOL      DSHOT600
#define DSHOT_BIDIR             true        /* Enable bidirectional DSHOT   */
#define DSHOT_IDLE_VALUE        550         /* ESC idle signal value        */
#define DIGITAL_IDLE_PERCENT    5.5f        /* % throttle at idle           */
#define USE_UNSYNCED_PWM        false

/*
 * Motor layout — standard Betaflight X-frame quad:
 *
 *     Front
 *  (4)   (2)
 *   CCW   CW
 *    \   /
 *     \ /
 *     / \
 *    /   \
 *   CW   CCW
 *  (1)   (3)
 *     Rear
 *
 *  M1 = Rear  Right  (CW  / bottom-side prop)
 *  M2 = Front Right  (CCW / top-side prop)
 *  M3 = Rear  Left   (CCW / top-side prop)
 *  M4 = Front Left   (CW  / bottom-side prop)
 */
typedef enum {
    MOTOR_1_REAR_RIGHT  = 0,
    MOTOR_2_FRONT_RIGHT = 1,
    MOTOR_3_REAR_LEFT   = 2,
    MOTOR_4_FRONT_LEFT  = 3,
    MOTOR_COUNT         = 4
} motorIndex_e;

/* ============================================================
 * SECTION 3 — PID CONFIGURATION
 * Tuned for EX1404 4800KV on a 3" airframe
 * ============================================================ */

typedef struct {
    uint8_t  P;
    uint8_t  I;
    uint8_t  D;
    uint16_t F;  /* Feed-forward */
} pidAxis_t;

typedef struct {
    pidAxis_t roll;
    pidAxis_t pitch;
    pidAxis_t yaw;

    /* Throttle PID Attenuation (TPA) */
    uint8_t  tpa_rate;           /* % reduction at full throttle */
    uint16_t tpa_breakpoint;     /* throttle value TPA starts    */
    uint8_t  tpa_mode;           /* 0 = PD, 1 = D only           */

    /* Anti-gravity — boosts I-term on fast throttle changes */
    uint16_t anti_gravity_gain;

    /* I-term relax — reduces bounce-back on snap rolls/flips */
    uint8_t  iterm_relax;        /* 0=OFF 1=RP 2=RPY            */
    uint8_t  iterm_relax_type;   /* 0=GYRO 1=SETPOINT           */
    uint8_t  iterm_relax_cutoff;
} pidProfile_t;

static const pidProfile_t pidProfile_ex1404_4800kv = {
    .roll  = { .P = 45, .I = 80,  .D = 38, .F = 90 },
    .pitch = { .P = 48, .I = 85,  .D = 40, .F = 90 },
    .yaw   = { .P = 45, .I = 90,  .D =  0, .F = 80 },

    /* TPA: reduce P/D above mid-throttle for high-KV motors */
    .tpa_rate       = 65,
    .tpa_breakpoint = 1600,
    .tpa_mode       = 1,    /* D-only TPA */

    /* Anti-gravity boosts I on punch — good for racing/freestyle */
    .anti_gravity_gain = 8000,

    /* I-term relax on Roll+Pitch in setpoint mode */
    .iterm_relax        = 1,
    .iterm_relax_type   = 1,
    .iterm_relax_cutoff = 15
};

/* ============================================================
 * SECTION 4 — RATES PROFILE
 * ============================================================ */

typedef struct {
    uint8_t rc_rate;
    uint8_t expo;
    uint8_t super_rate;  /* "srate" — controls center feel    */
} axisRate_t;

typedef struct {
    axisRate_t roll;
    axisRate_t pitch;
    axisRate_t yaw;
} rateProfile_t;

static const rateProfile_t rateProfile_default = {
    .roll  = { .rc_rate = 105, .expo = 15, .super_rate = 70 },
    .pitch = { .rc_rate = 105, .expo = 15, .super_rate = 70 },
    .yaw   = { .rc_rate = 100, .expo = 10, .super_rate = 40 }
};

/* ============================================================
 * SECTION 5 — FILTER CONFIGURATION
 * RPM filtering is the primary noise reduction strategy here.
 * High-KV motors (4800KV) produce high-frequency noise that
 * RPM filters track dynamically far better than static notches.
 * ============================================================ */

typedef struct {
    /* Gyro lowpass filters */
    uint16_t gyro_lowpass_hz;       /* Primary   */
    uint16_t gyro_lowpass2_hz;      /* Secondary */
    uint8_t  gyro_lowpass_type;     /* 0=PT1 1=BIQUAD */
    uint8_t  gyro_lowpass2_type;

    /* Static notch filters (disabled when RPM filter active) */
    uint16_t gyro_notch1_hz;
    uint16_t gyro_notch2_hz;

    /* Dynamic notch filter */
    uint8_t  dyn_notch_count;
    uint16_t dyn_notch_q;
    uint16_t dyn_notch_min_hz;
    uint16_t dyn_notch_max_hz;

    /* D-term filters */
    uint16_t dterm_lowpass_hz;
    uint16_t dterm_lowpass2_hz;

    /* RPM filter (requires bidirectional DSHOT) */
    bool     rpm_filter_enabled;
    uint8_t  rpm_filter_harmonics;  /* Number of harmonic notches */
    uint16_t rpm_filter_min_hz;
} filterConfig_t;

static const filterConfig_t filterConfig_ex1404 = {
    /* Gyro — moderate lowpass, stiff 3" airframe tolerates higher cutoff */
    .gyro_lowpass_hz    = 200,
    .gyro_lowpass2_hz   = 300,
    .gyro_lowpass_type  = 1,    /* BIQUAD  */
    .gyro_lowpass2_type = 0,    /* PT1     */

    /* Static notches off — RPM filter handles motor harmonics */
    .gyro_notch1_hz     = 0,
    .gyro_notch2_hz     = 0,

    /* Dynamic notch — secondary safety net for frame resonance */
    .dyn_notch_count    = 3,
    .dyn_notch_q        = 300,
    .dyn_notch_min_hz   = 100,
    .dyn_notch_max_hz   = 600,

    /* D-term filters */
    .dterm_lowpass_hz   = 100,
    .dterm_lowpass2_hz  = 200,

    /* RPM filter — tracks motor noise across all throttle positions */
    .rpm_filter_enabled   = true,
    .rpm_filter_harmonics = 3,
    .rpm_filter_min_hz    = 100
};

/* ============================================================
 * SECTION 6 — RECEIVER CONFIGURATION
 * Default: ExpressLRS (ELRS) on UART2 via CRSF protocol
 * Adjust uart_port if your RX is wired to a different UART.
 * ============================================================ */

typedef enum {
    SERIALRX_SPEKTRUM1024  = 0,
    SERIALRX_SPEKTRUM2048  = 1,
    SERIALRX_SBUS          = 2,
    SERIALRX_SUMD          = 3,
    SERIALRX_SUMH          = 4,
    SERIALRX_XBUS_MODE_B   = 5,
    SERIALRX_XBUS_MODE_B_RJ01 = 6,
    SERIALRX_IBUS          = 7,
    SERIALRX_JETIEXBUS     = 8,
    SERIALRX_CRSF          = 9,    /* ← ExpressLRS / TBS Crossfire */
    SERIALRX_SRXL          = 10,
    SERIALRX_FPORT         = 11,
    SERIALRX_SRXL2         = 12,
    SERIALRX_GHST          = 13
} rxProvider_e;

typedef struct {
    rxProvider_e provider;
    uint8_t      uart_port;     /* 1-indexed UART number    */
    uint32_t     baud_rate;
    bool         inverted;
    bool         half_duplex;
} rxSerialConfig_t;

static const rxSerialConfig_t rxConfig = {
    .provider    = SERIALRX_CRSF,
    .uart_port   = 2,           /* UART2 — adjust to match wiring */
    .baud_rate   = 420000,      /* ELRS default CRSF baud        */
    .inverted    = false,
    .half_duplex = false
};

/* ============================================================
 * SECTION 7 — BATTERY / CURRENT MONITORING
 * SpeedyBee F405 AIO has onboard voltage + current sensing.
 * ============================================================ */

typedef struct {
    uint16_t ibata_scale;               /* Current sensor scale      */
    uint8_t  vbat_scale;                /* Voltage sensor scale      */
    uint8_t  vbat_min_cellvoltage;      /* 3.30V per cell (×100)     */
    uint8_t  vbat_max_cellvoltage;      /* 4.20V per cell (×100)     */
    uint8_t  vbat_warning_cellvoltage;  /* 3.50V per cell warn (×100)*/
    uint8_t  vbat_hysteresis;
    uint16_t bat_capacity_mah;          /* 0 = auto-detect           */
} batteryConfig_t;

static const batteryConfig_t battConfig = {
    .ibata_scale               = 400,
    .vbat_scale                = 110,
    .vbat_min_cellvoltage      = 330,   /* 3.30V/cell — LiPo cutoff  */
    .vbat_max_cellvoltage      = 420,   /* 4.20V/cell — full charge  */
    .vbat_warning_cellvoltage  = 350,   /* 3.50V/cell — warn in OSD  */
    .vbat_hysteresis           = 1,
    .bat_capacity_mah          = 650    /* Set to your actual mAh    */
};

/* ============================================================
 * SECTION 8 — OSD CONFIGURATION
 * Built-in AT7456E OSD chip on the F405 AIO
 * ============================================================ */

typedef struct {
    bool     enabled;
    uint8_t  units;             /* 0=IMPERIAL 1=METRIC           */
    uint8_t  rssi_alarm;        /* % — warn below this RSSI      */
    uint16_t cap_alarm;         /* mAh consumed warning level    */

    /* Element screen positions (grid-based, 30×13 for PAL) */
    uint16_t pos_vbat;
    uint16_t pos_rssi;
    uint16_t pos_timer2;
    uint16_t pos_flymode;
    uint16_t pos_throttle;
    uint16_t pos_current;
    uint16_t pos_mah_drawn;
    uint16_t pos_craft_name;
    uint16_t pos_altitude;
} osdConfig_t;

static const osdConfig_t osdConfig = {
    .enabled    = true,
    .units      = 1,            /* METRIC                        */
    .rssi_alarm = 25,
    .cap_alarm  = 1200,

    /* Positions — customize in BF Configurator OSD tab */
    .pos_vbat       = 2422,
    .pos_rssi       = 2456,
    .pos_timer2     = 2489,
    .pos_flymode    = 2425,
    .pos_throttle   = 2458,
    .pos_current    = 2390,
    .pos_mah_drawn  = 2391,
    .pos_craft_name = 2058,
    .pos_altitude   = 2339
};

/* ============================================================
 * SECTION 9 — FAILSAFE CONFIGURATION
 * ============================================================ */

typedef enum {
    FAILSAFE_PROCEDURE_AUTO_LANDING = 0,
    FAILSAFE_PROCEDURE_DROP         = 1,    /* ← recommended for racing */
    FAILSAFE_PROCEDURE_GPS_RESCUE   = 2
} failsafeProcedure_e;

typedef struct {
    uint8_t             delay;              /* ×0.1s before activating  */
    uint8_t             recovery_delay;     /* ×0.1s for signal recovery*/
    uint8_t             off_delay;          /* ×0.1s motor-off delay    */
    uint16_t            throttle;           /* throttle during failsafe */
    bool                kill_switch;
    uint8_t             throttle_low_delay;
    failsafeProcedure_e procedure;
} failsafeConfig_t;

static const failsafeConfig_t failsafeConfig = {
    .delay              = 4,    /* 0.4s after signal loss → failsafe */
    .recovery_delay     = 20,
    .off_delay          = 200,
    .throttle           = 1000,
    .kill_switch        = false,
    .throttle_low_delay = 100,
    .procedure          = FAILSAFE_PROCEDURE_DROP
};

/* ============================================================
 * SECTION 10 — AUX MODE ASSIGNMENTS
 * Customize channel numbers to match your transmitter layout.
 * ============================================================ */

typedef struct {
    uint8_t  modeId;    /* Betaflight flight mode ID             */
    uint8_t  auxChannel;/* AUX channel (0-indexed, AUX1=0, etc.) */
    uint16_t rangeMin;  /* Activate when channel > rangeMin      */
    uint16_t rangeMax;  /* Deactivate when channel > rangeMax    */
} modeActivationCondition_t;

static const modeActivationCondition_t modeConfig[] = {
    /* ARM          — AUX1 HIGH  */
    { .modeId = 0,  .auxChannel = 0, .rangeMin = 1700, .rangeMax = 2100 },
    /* ANGLE        — AUX2 LOW   */
    { .modeId = 1,  .auxChannel = 1, .rangeMin = 900,  .rangeMax = 1300 },
    /* BEEPER       — AUX3 HIGH  */
    { .modeId = 13, .auxChannel = 2, .rangeMin = 1700, .rangeMax = 2100 },
    /* TURTLE MODE  — AUX4 HIGH  */
    { .modeId = 35, .auxChannel = 3, .rangeMin = 1700, .rangeMax = 2100 }
};

/* ============================================================
 * SECTION 11 — FEATURE FLAGS
 * ============================================================ */

#define FEATURE_RX_SERIAL       (1 << 2)
#define FEATURE_MOTOR_STOP      (1 << 4)
#define FEATURE_SOFTSERIAL      (1 << 6)
#define FEATURE_TELEMETRY       (1 << 10)   /* Disabled — enable if needed */
#define FEATURE_OSD             (1 << 18)

#define ENABLED_FEATURES (                  \
    FEATURE_RX_SERIAL   |                  \
    FEATURE_MOTOR_STOP  |                  \
    FEATURE_SOFTSERIAL  |                  \
    FEATURE_OSD                            \
)

/* ============================================================
 * SECTION 12 — CLI COMMAND GENERATOR
 * Prints all CLI commands to stdout — pipe to serial port or
 * paste directly into Betaflight Configurator CLI tab.
 * ============================================================ */

static void generateCLICommands(void) {

    printf("# === SpeedyBee F405 AIO + EX1404 4800KV ===\n");
    printf("# Generated by speedybee_f405_ex1404_config.c\n\n");

    /* Board */
    printf("board_name %s\n", BOARD_NAME);
    printf("manufacturer_id %s\n\n", MANUFACTURER_ID);

    /* Motor / ESC */
    printf("# --- Motor / ESC ---\n");
    printf("set motor_pwm_protocol = DSHOT600\n");
    printf("set dshot_bidir = ON\n");
    printf("set dshot_idle_value = %d\n",    DSHOT_IDLE_VALUE);
    printf("set digital_idle_percent = %.1f\n", DIGITAL_IDLE_PERCENT);
    printf("set motor_poles = %d\n",         MOTOR_POLE_COUNT);
    printf("set use_unsynced_pwm = OFF\n\n");

    /* PIDs */
    printf("# --- PID Profile ---\nprofile 0\n");
    printf("set p_roll = %d\nset i_roll = %d\nset d_roll = %d\nset f_roll = %d\n",
           pidProfile_ex1404_4800kv.roll.P,
           pidProfile_ex1404_4800kv.roll.I,
           pidProfile_ex1404_4800kv.roll.D,
           pidProfile_ex1404_4800kv.roll.F);
    printf("set p_pitch = %d\nset i_pitch = %d\nset d_pitch = %d\nset f_pitch = %d\n",
           pidProfile_ex1404_4800kv.pitch.P,
           pidProfile_ex1404_4800kv.pitch.I,
           pidProfile_ex1404_4800kv.pitch.D,
           pidProfile_ex1404_4800kv.pitch.F);
    printf("set p_yaw = %d\nset i_yaw = %d\nset d_yaw = %d\nset f_yaw = %d\n",
           pidProfile_ex1404_4800kv.yaw.P,
           pidProfile_ex1404_4800kv.yaw.I,
           pidProfile_ex1404_4800kv.yaw.D,
           pidProfile_ex1404_4800kv.yaw.F);
    printf("set tpa_rate = %d\nset tpa_breakpoint = %d\nset tpa_mode = D\n",
           pidProfile_ex1404_4800kv.tpa_rate,
           pidProfile_ex1404_4800kv.tpa_breakpoint);
    printf("set anti_gravity_gain = %d\n",   pidProfile_ex1404_4800kv.anti_gravity_gain);
    printf("set iterm_relax = RP\nset iterm_relax_type = SETPOINT\n");
    printf("set iterm_relax_cutoff = %d\n\n", pidProfile_ex1404_4800kv.iterm_relax_cutoff);

    /* Rates */
    printf("# --- Rate Profile ---\nrateprofile 0\n");
    printf("set roll_rc_rate = %d\nset roll_expo = %d\nset roll_srate = %d\n",
           rateProfile_default.roll.rc_rate,
           rateProfile_default.roll.expo,
           rateProfile_default.roll.super_rate);
    printf("set pitch_rc_rate = %d\nset pitch_expo = %d\nset pitch_srate = %d\n",
           rateProfile_default.pitch.rc_rate,
           rateProfile_default.pitch.expo,
           rateProfile_default.pitch.super_rate);
    printf("set yaw_rc_rate = %d\nset yaw_expo = %d\nset yaw_srate = %d\n\n",
           rateProfile_default.yaw.rc_rate,
           rateProfile_default.yaw.expo,
           rateProfile_default.yaw.super_rate);

    /* Filters */
    printf("# --- Filters ---\n");
    printf("set gyro_lowpass_hz = %d\n",     filterConfig_ex1404.gyro_lowpass_hz);
    printf("set gyro_lowpass2_hz = %d\n",    filterConfig_ex1404.gyro_lowpass2_hz);
    printf("set gyro_lowpass_type = BIQUAD\nset gyro_lowpass2_type = PT1\n");
    printf("set gyro_notch1_hz = 0\nset gyro_notch2_hz = 0\n");
    printf("set dyn_notch_count = %d\n",     filterConfig_ex1404.dyn_notch_count);
    printf("set dyn_notch_q = %d\n",         filterConfig_ex1404.dyn_notch_q);
    printf("set dyn_notch_min_hz = %d\n",    filterConfig_ex1404.dyn_notch_min_hz);
    printf("set dyn_notch_max_hz = %d\n",    filterConfig_ex1404.dyn_notch_max_hz);
    printf("set dterm_lowpass_hz = %d\n",    filterConfig_ex1404.dterm_lowpass_hz);
    printf("set dterm_lowpass2_hz = %d\n",   filterConfig_ex1404.dterm_lowpass2_hz);
    printf("set rpm_filter_harmonics = %d\n",filterConfig_ex1404.rpm_filter_harmonics);
    printf("set rpm_filter_min_hz = %d\n\n", filterConfig_ex1404.rpm_filter_min_hz);

    /* Battery */
    printf("# --- Battery ---\n");
    printf("set ibata_scale = %d\n",            battConfig.ibata_scale);
    printf("set vbat_scale = %d\n",             battConfig.vbat_scale);
    printf("set vbat_min_cellvoltage = %d\n",   battConfig.vbat_min_cellvoltage);
    printf("set vbat_max_cellvoltage = %d\n",   battConfig.vbat_max_cellvoltage);
    printf("set vbat_warning_cellvoltage = %d\n",battConfig.vbat_warning_cellvoltage);
    printf("set bat_capacity = %d\n\n",         battConfig.bat_capacity_mah);

    /* Failsafe */
    printf("# --- Failsafe ---\n");
    printf("set failsafe_delay = %d\n",             failsafeConfig.delay);
    printf("set failsafe_recovery_delay = %d\n",    failsafeConfig.recovery_delay);
    printf("set failsafe_off_delay = %d\n",         failsafeConfig.off_delay);
    printf("set failsafe_throttle = %d\n",          failsafeConfig.throttle);
    printf("set failsafe_kill_switch = OFF\n");
    printf("set failsafe_throttle_low_delay = %d\n",failsafeConfig.throttle_low_delay);
    printf("set failsafe_procedure = DROP\n\n");

    /* RX */
    printf("# --- Receiver (ELRS/CRSF on UART%d) ---\n", rxConfig.uart_port);
    printf("set serialrx_provider = CRSF\n");
    printf("set serialrx_inverted = OFF\n");
    printf("set serialrx_halfduplex = OFF\n");
    printf("serial %d 64 115200 57600 0 115200\n\n", rxConfig.uart_port - 1);

    /* AUX Modes */
    printf("# --- Mode/AUX assignments ---\n");
    int numModes = (int)(sizeof(modeConfig) / sizeof(modeConfig[0]));
    for (int i = 0; i < numModes; i++) {
        printf("aux %d %d %d %d %d 0 0\n",
               i,
               modeConfig[i].modeId,
               modeConfig[i].auxChannel,
               modeConfig[i].rangeMin,
               modeConfig[i].rangeMax);
    }

    /* OSD */
    printf("\n# --- OSD ---\n");
    printf("set osd_units = METRIC\n");
    printf("set osd_rssi_alarm = %d\n",     osdConfig.rssi_alarm);
    printf("set osd_cap_alarm = %d\n",      osdConfig.cap_alarm);
    printf("set osd_vbat_pos = %d\n",       osdConfig.pos_vbat);
    printf("set osd_rssi_pos = %d\n",       osdConfig.pos_rssi);
    printf("set osd_flymode_pos = %d\n",    osdConfig.pos_flymode);
    printf("set osd_current_pos = %d\n",    osdConfig.pos_current);
    printf("set osd_mah_drawn_pos = %d\n",  osdConfig.pos_mah_drawn);
    printf("set osd_craft_name_pos = %d\n", osdConfig.pos_craft_name);
    printf("set osd_altitude_pos = %d\n\n", osdConfig.pos_altitude);

    printf("save\n");
    printf("# ============================================================\n");
    printf("# Configuration complete. FC will reboot after 'save'.\n");
    printf("# Remember to: calibrate accel, verify motor directions,\n");
    printf("# enable bidirectional DSHOT in BLHeli32 ESC firmware.\n");
    printf("# ============================================================\n");
}

/* ============================================================
 * SECTION 13 — ENTRY POINT
 * Compile and run to generate CLI commands:
 *   gcc speedybee_f405_ex1404_config.c -o fc_config
 *   ./fc_config > cli_commands.txt
 * Then paste cli_commands.txt into Betaflight CLI tab.
 * ============================================================ */

int main(void) {
    generateCLICommands();
    return 0;
}
