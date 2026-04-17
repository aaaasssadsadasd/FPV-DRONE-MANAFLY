# Journal #5 test print, goggles acquired, changed FC
Ran a test print with standard pla filament instead of the petg. Don't have that on hand yet but soon will. The test came out great, better than I expected. There were more supports than actual drone frame. Satisfied with current print, took around 4 hours. Will be using this as the guinea pig for wiring and component placements while getting hands on petg filament.

<img width="3024" height="4032" alt="unnamed" src="https://github.com/user-attachments/assets/ae5cb823-6778-46fa-a20f-6b19e4d1c9b1" />

I purchased a pair of knock-off ev 800d goggles because I felt the need to commit to this project and not rely on hack club funding. It came out to a total of $60, not bad.
<img width="4032" height="3024" alt="IMG_6873" src="https://github.com/user-attachments/assets/3dfecab4-9890-42b2-977b-21fccd345747" />
<img width="3024" height="4032" alt="IMG_6874" src="https://github.com/user-attachments/assets/f7089dea-f260-4c73-beaf-aa2fe8681c4f" />



However, since it was a cheap knockoff, it requires an external power source. This means I'll have to strap a battery to it or drag it around connected from a wire.

Also, the speedybee f405 AIO was so popular it went up by $30 on aliexpress. Every listing of it was $80-$100, so I found an alternative:

SpeedyBee F405 BLS V2 Mini BLS ESC Stack – 20x20


<img width="1071" height="875" alt="Screenshot 2026-04-16 223403" src="https://github.com/user-attachments/assets/a3475215-beec-452f-a574-1dbd4d4f26a1" />

Usually, this would be around $14 more expensive than the f405 AIO but aliexpress is probably having a special event right now. It is the cheapest compatible flight controller I've seen and the only downside is slightly worse cooling and a bit more space occupied. This should be fine as the drone frame seats 20x20 and 25x25 flight controllers. I hope the price doesn't raise again while awaiting approval...

This entry covers the past 4 days as I couldn't write while the project was waiting for approval.


# Journal #4
CAD
The stl file for the drone chassis was upscaled by x10 when first imported. It took a while to figure out how to rescale it. After joining a call with the designer of the model, we resolved the issue. Added components such as camera, motors, battery, a 25 x 25mm square represents the flight controller in place.

<img width="1127" height="779" alt="Screenshot 2026-04-12 040009" src="https://github.com/user-attachments/assets/5eb7c83d-c5bf-48c9-a09b-9fdfb5c1766f" />


Original step file:

<img width="1011" height="635" alt="Screenshot 2026-04-12 040514" src="https://github.com/user-attachments/assets/6dfe9b5e-3cb6-476e-8c61-1b3dcfff9081" />


LENGTH: 135 MM WIDTH: 135 MM


# Journal #3

changes to BOM, hopefully done for good.
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

# Journal 2
parts search
Finished compiling a complete BOM, part search process was time consuming, I did my best to find the most affordable options. Current total for the drone build itself is around $180, goggles and controller will bring it up to near $300. Changes will be made, for lowering total cost and I am willing to pay out of pocket for the controller and propellors. I've gone and purchased propellors already. I will have a friend helping in on this project, it is the most ambitious project so far for the both of us. Wish us luck.

# Journal 1
Parts search
Parts searching. looking for budget options, optimal find with the flight controller and battery.

SpeedyBee F405 AIO 40A. $55

4S 650mAh LiPo (XT30) $17

more parts searching tomorrow...

<img width="843" height="510" alt="image" src="https://github.com/user-attachments/assets/85f3b165-4aeb-461f-a06b-40e79220cee1" />
