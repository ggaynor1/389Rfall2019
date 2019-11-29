# Writeup 1 - Web I

Name: George Gaynor
Section: 0101

I pledge on my honor that I have not given or received any unauthorized assistance on this assignment or examination.

Digital acknowledgement: George Gaynor


## Assignment details
This assignment has two parts. It is due by 11/28/19 at 11:59PM.

**There will be late penalty of 5% per day late!**

### Part 1 (40 Pts)

Such a Quick Little, website!

[http://142.93.136.81:5000/](http://142.93.136.81:5000/)

The flag here is CMCS389R-{h1dd3n_1n_plain_5ight}. I found this by removing the port from the URL, then inspecting the html source code for the front page of the 'Elite Banking' website.

### Part 2 (60 Pts)
Complete all 6 levels of:

[https://xss-game.appspot.com](https://xss-game.appspot.com)

Produce a writeup. We will not take off points for viewing the source code and/or viewing hints, but we strongly discourage reading online write-ups as that defeats the purpose of the homework.

1. For level 1, I injected an html script tag, <script>alert('0')</script>, to generate the alert box.
2. For level 2, simply injecting an html script tag into the text box did not work. In order to get around this, I used a javascript element with the hyperlink tag and the href attribute to generate the alert. <a href="javascript:alert('0')">?</a>
3. For this level, I was working directly with the address bar. <a href> was not working since it was reliant on me being able to click the generated hyperlink. Instead, I used 'onerror='alert(1)', with onerror being an attribute for the <img> tag. The script for this level makes use of the <img> tag to generate the images. 
4. For level 4, it required using escaping inputs after ?timer=. I opened the error console and entered a single quote, as instructed by the hint and noticed ")" was required as an argument. I ended up trying ?timer='); to terminate the argument. After much trial and error, I escaped the semi-colon using %3B, and used ')%3Balert(5)%3Bvar a=('.
5. For level 5, upon inspecting the source code, I see that the email field is never used and the result is reliant upon the next button and the URL. Therefore, I changed signup?next=confirm to signup?next=javascript:alert(1). 
6. For level 6, I first removed the file extension from gadget.js. This resulted in output of 'Couldn't load gadget from /static/gadget. This reveals that the file is being loaded externally. If I remove the entire directory, /static/gadget.js, the page is returned to normal, resulting in output of Loaded gadget from /static/gadget.js. As a result I went on pastebin and created a raw file with 'alert(1);' being the only data in the file. I first loaded the raw from https://pastebin.com/raw/PEAhXrRT, which originally failed. After inspecting the source code, I saw that there was a regular expression check for an exact match of https. I changed the lowercase h to and uppercase H and was able produce the alert().

### Format

Part 1 and 2 can be answered in bullet form or full, grammatical sentences.

### Scoring

* Part 1 is worth 40 points
* Part 2 is worth 60 points

### Tips

Remember to document your thought process for maximum credit!

Review the slides for help with using any of the tools or libraries discussed in
class.
