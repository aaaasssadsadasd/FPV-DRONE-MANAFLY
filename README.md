# FPV-DRONE-MANAFLY
3" drone project based on 3d printed chasis by manafly. Modern drone chassis are usually always made from carbon fiber, it is light and durable. Using ai generative design, this drone frame is optimized for weight and strength. It's design allows it to perform competitively with carbon fiber frames while being printed from PETG or ABS filament. Each frame costs around $1 in filament. This is the foundation for my project, however the frame is the cheapest part of it all.

<img width="789" height="690" alt="Screenshot 2026-04-11 162020" src="https://github.com/user-attachments/assets/d8799b87-02b3-4b97-a9c9-50224d09d091" />


# BOM
# FPV Drone with 3D Printed Chassis - Bill of Materials

| Name | Purpose | Quantity | Total Cost (USD) | Distributor |
|------|---------|----------|------------------|-------------|
| EV 800D EACHINE | FPV GOGGLES | — | $50.00 | Facebook Marketplace |
| RADIOMASTER T8L (M2) | RADIO CONTROLLER | — | $50.00 | Amazon |
| OVONIC 4S Lipo Battery 80C 850mAh 14.8V | BATTERY | — | $18.00 | Amazon |
| RADIOMASTER ELRS RECIEVER RP1 | ELRS RECIEVER | — | $21.00 | Amazon |
| SOLOGOOD CADDX ANT | FPV CAMERA | — | $20.00 | Amazon |
| ZEUS MINI VTX | VIDEO TRAMSMITTER | — | $25.00 | Amazon |
| HAPPYMODEL 1404 KV4800 4S | MOTORS | 4 | $28.00 | AliExpress |
| SpeedyBee F405 AIO | FLIGHT CONTROLLER | — | $59.00 | AliExpress |

## Summary

**Estimated Subtotal Cost:** $271.00

### Components by Distributor
- **Amazon:** 5 items
- **AliExpress:** 2 items
- **Facebook Marketplace:** 1 item

# Journal #3

changes to BOM
The speedybee f405 AIO was sold out in many places i searched including the official website. I only managed to find a link through a review blog. On Amazon and Ebay it was marked up to x2 the price. I wanted to find it near its original price of 50-60 and luckily I found it.

https://oscarliang.com/speedybee-f405-aio-fc/#Where-to-Buy

I spent a while debating whether or not to get the radiomaster t8l controller from the official website or thru amazon. On the website the shipping + tax made it out to around the same price as If i were to buy it on amazon for a markup. I decided on amazon to simplify the buying process.

I also did some better research on the motors. Instead of buying from an individual site I went on aliexpress and chose a similarly priced but overall better set of 4 motors.

After some digging through the options, The best pick (KV4800) became apparent, it offered the best fit for a 3 inch micro build and gives the most responsive performance out of the options. at 14.8V it matched the batteries. The 4800KV had the highest rpm per volt, fastest top speed, but least torque when compared to the 3500KV and 2750KV options.

I am running the gemfan 3020 propellors. Good match.

<img width="948" height="220" alt="Screenshot 2026-04-11 162536" src="https://github.com/user-attachments/assets/8f5c6316-1e67-404d-b706-0f77d36855ff" />
<img width="691" height="871" alt="Screenshot 2026-04-11 175723" src="https://github.com/user-attachments/assets/b6963601-10e9-4b96-a138-cd2627d17def" />


https://www.aliexpress.us/item/3256804892488196.html?spm=a2g0o.cart.0.0.850b38dah8xcmd&mp=1&pdp_npi=6%40dis%21USD%21USD%2055.80%21USD%2027.90%21%21USD%2027.90%21%21%21%402101e2b417759553661585320ef61c%2112000031557507513%21ct%21US%217412077187%21%211%210%21&pdp_ext_f=%7B%22cart2PdpParams%22%3A%7B%22pdpBusinessMode%22%3A%22retail%22%7D%7D&gatewayAdapt=glo2usa

The tallest task today was finding a cheap and suitable pair of goggles, After asking around, i was reccomended the EV 800D goggles from eachine. It retailed for above 120 USD. After long hours of searching. I landed on a second-hand for only $50 on facebook marketplace. (shipping not included)

The radiomaster t8l (controller) and the RP1 receiver both use the ELRS 2.4Ghz protocol and run on 2.400-2.480Ghz frequency. RF chip SX1281 on both. CRSF data bus.

The EV800D is an analog 5.8 GHz receiver, compatible with the SoloGood Caddx ANT camera (analog), along with the HGLRC Zeus Mini VTX which is an analog 5.8GHz transmitter. This makes for our video chain.
<img width="1342" height="715" alt="Screenshot 2026-04-11 180432" src="https://github.com/user-attachments/assets/0c566f67-7b49-4bc7-9fe5-01be521db7f3" />

<img width="1146" height="1072" alt="Screenshot 2026-04-11 173555" src="https://github.com/user-attachments/assets/fea75044-02c5-4c66-a430-a1b430953b85" />
<img width="1326" height="844" alt="Screenshot 2026-04-11 172832" src="https://github.com/user-attachments/assets/2eba274c-6118-47fd-95e8-ed117be67207" />
