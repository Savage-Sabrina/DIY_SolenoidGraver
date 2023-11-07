For a full in depth review of the solenoid choices please watch the video about the perks and configuration of each magnet below. [Youtube video about the solenoids and handpiece](https://www.youtube.com/watch?v=2lFhfABcx04)

The first solenoid I tested was the smallest 5v solenoid i could get just to prove the concept. [Small solenoid](https://www.adafruit.com/product/2776)
![small 5V solenoid](https://user-images.githubusercontent.com/13701387/123425489-8106a480-d5c2-11eb-8b57-86754660a24c.jpg)

It was really weak and I realized that I was supposed to know about how a graver would work.
I found some options to understand better the impact force on this video from [Shaun Hughes](https://www.youtube.com/watch?v=1zzqaAYdvdQ&ab_channel=ShaunHughesEngraving) and from that went on the look for a solenoid that would give me the aproximate ammount of travel for this to happen.

The first solenoid I went to was a popular [jf-0530B](https://www.aliexpress.com/item/32777233179.html) (12V, 300mA) that had quite a punch but a long stroke and long body, making the handpice too bulky. Maybe to replace a hammer and chisel for someone with a hand disabilyty might be awesome.
![jf-0530b](https://user-images.githubusercontent.com/13701387/123425675-bad7ab00-d5c2-11eb-97bb-10ff525da788.jpg)


The second solenoid used was the one from [Adafruit](https://www.adafruit.com/product/413),with 12V and 250mA it was a little wider. It had strong strokes but a lower frequency of oscilation. (https://www.adafruit.com/product/413)
![adafruit solenoid](https://user-images.githubusercontent.com/13701387/123426601-e4dd9d00-d5c3-11eb-9a87-a4682eb55a6b.jpg)


I had an intermediary solenoid (12V, ~900mA) that was open and could dissipate better the heat while having a good impact force. Like the on on the [link](https://www.aliexpress.com/item/4000013441378.html?spm=a2g0o.productlist.0.0.3cf711e0Wxrr2f&algo_pvid=70f2bb48-4aba-4778-9bd3-d017993e9e11&algo_expid=70f2bb48-4aba-4778-9bd3-d017993e9e11-27&btsid=0b0a0ae215981717711361404eaefc&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_), unfortunatelly this also had a low frequency and to make it fit on an easy handpiece was a hassle.

Used a [linear cylindrical solenoid](https://www.red-magnetics.com/en/product-groups/linear-solenoids/cylinder-solenoids/its-lz-1335-d/) to have better control on the stroke length and natural frequency of the solenoid. Got this one on the 12V and 24V variants. After testing for some time I realized my tolerances were too tight as the shaft had bended and shortened with the impact. I would need something more robust as an impact shaft to withstand the continuous impacts on the solenoid.

I had to find an intermitent duty, 12-24V solenoid that would be good to use and easy to maintain. Found a [perfect match](https://www.youtube.com/watch?v=ASwVTQaLLGc&t=62s&ab_channel=IndexBraille) with no avail to get my hands on it as the complay would not sell unless I had the machine that would use it. So I had to find something similar on the internet. After going back and forth with the manufacturer I got a [replacement solenoid](https://www.aliexpress.com/item/32996508114.html?spm=a2g0s.9042311.0.0.53844c4ddj9pqp) that would be the same function as the perfect match I wanted. Cut the shaft to length and got a better frequency of oscilation on the solenoid and better impact force with 19V. But it would still overheat. Thats were I came to realization that it's better to make my own solenoid. The saft from the cheap solenoid is percet to make my own. And this is the next step on the making of the handpiece.
![Screenshot 2021-06-25 150734](https://user-images.githubusercontent.com/13701387/123429532-5703b100-d5c7-11eb-9be2-bdbae92c3062.png)




Extras:
How to make a solenoid: https://www.instructables.com/How-to-Make-Solenoid-at-Home/
Mosfet control for NERF: https://www.youtube.com/watch?v=hRxzcRwo_3U&ab_channel=Valour%27sBlasters
