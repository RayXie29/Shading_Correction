#Lens shading/vignetting correction algorithm<br />
This repo is the implementation of Patent US20070211154 which is the lens vignetting correction algorithm. <br />
Vigetting is a pheonmenon that the brightness is decreasing from image center to corner. This pheonmenon might due to design of camera lens or camera setting. <br />
This patent is using a radially symmetric polynomial function to correct the issue. This might because the vignetting effect is happened radially symmertic from center to corner. <br />
<br />
<br />
First we need to use a raw image to calculate the paramteres of correction function. This raw image is from the camera system you want to correct and it doesnt have any ISP pipeline processing yet. If your camera is using color sensor, then the raw image should be a bayer image. Normally this raw image should take an flat uniform light source. We can from the image of your camera taking flat uniform light source to know the vignetting  situation. And use this situation to calibrate the function parameters. <br />

Here is my raw image, resolution : 640x480 , bayer pattern : BGGR <br />
![alt text](https://raw.githubusercontent.com/RayXie29/Shading_Correction/master/imgs/unShading.bmp)
<br />
We use this raw image to calculate the function parameters -> "ShadingParameters.csv" <br />
Then we use the correction function to correct the image, here is the result <br />
![alt text](https://raw.githubusercontent.com/RayXie29/Shading_Correction/master/imgs/result.bmp)
<br />
Then we do some color interpolation and correction on them to make the images normal.<br />
Here is the original image after color interpolation and correction<br />
![alt text](https://raw.githubusercontent.com/RayXie29/Shading_Correction/master/imgs/original.bmp)
<br />
And here is the image after shading correction and color interpolation/correction<br />
![alt text](https://raw.githubusercontent.com/RayXie29/Shading_Correction/master/imgs/corrected.bmp)

<br />
I also used the corrected image to subtract the original image to see the correction effect. From this difference we can see the image region close to center has very few change. But the corner part, which is the serious vignetting region has very effective change.<br />

![alt text](https://raw.githubusercontent.com/RayXie29/Shading_Correction/master/imgs/diff.bmp)
<br />
reference:<br />
https://patents.google.com/patent/US20070211154
