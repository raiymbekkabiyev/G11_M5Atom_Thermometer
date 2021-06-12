# G11_M5Atom_Thermometer
<h2> Description of the project </h2> 
<p>
 In this project, we worked with M5 Atom Matrix, a programmable device that has 5X5 RGB LED matrix and an IMU sensor. The goal in this project was to create a smart thermometer device with the following modes: <br> 
<ol> 
<li> Show Active temperature + Units </li>
<li> Show average of last 24 hours of temperature + Units </li>
<li> Show color scale of temperature range + current temperature as color </li>
<li> Show graph of temperature across a predefined range </li>
<li> Change units </li>
</ol>
The devise is constantly reading temperature data using IMU sensor. That allows the device to calculate average temperature for the last 24 hours. In this scenario, the orientation of the devise plays important role. Initially the devise is facing down (sleep mode) but it keeps reading data from the IMU. At any point (mode), device can be returned back to sleep mode by facing it down. <br> 
To enter the Active mode, user has to place the device facing up (to the ceiling) and press the button. This would also activate the first mode - show active temperature. Temperature is shown in Celcius by default (can be changed to Fahrenheit in mode 5). Now from Active mode, user can enter other modes by tilting the devise left or right. 
</p>
<h2> How do we scroll text to display temperature or why you need <strong> Adafruit </strong> library </h2> 
<p> Firsly, we want to give credits to Mike Klepper for making an incredible article on Advanced Display Usage, where we learnt how to use <strong> Adafruit </strong> library. Link to Mike's post on ATOM Matrix: Advanced Display Usage: https://patriot-geek.blogspot.com/2020/05/atom-matrix-advanced-display-usage.html
In order to scroll text on the M5Atom, one must add the following three libraries: 
<ol>
<strong>
<li> Adafruit_GFX.h </li>
<li> Adafruit_NeoMatrix.h </li>
<li> Adafruit_NeoPixel.h </li>  
</strong>
<ol>
Since most of the fonts in Adafruit library are designed for a bigger display, it is recommended to incldude <strong> Fonts/TomThumb.h </strong> font.
</p> 
<h2> How to use Accelerometer to identify orientation of M5Atom </h2> 
