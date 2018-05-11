# CSC621_TeamSky

## Description

This application applies ITK image processing algorithms.<br />

## Usage

Run application from Build/Debug/TeamSky.exe [args]<br />
Options/arguments must be written in order.<br />

Possible args are:<br />

| Option | Description |
| --- | --- |
| \-h, \-\-help | Display help menu |
| \-r | Registration |
| \-s image threshold level [labelVal] | Watershed Segmentation on image with specified threshold and level. Remove labels who have a membership count less than labelVal, defaults to zero. |
| \-3d image_dir num | Create 3D image from multiple 2D images. image_dir is a directory containing images named like 000000.dcm. num is the number of images you want to use in that directory from 000000.dcm through 00000N.dcm |
| \-f filter_name [args] | Apply generic filter to image. Optional args may be required by different filter types. Filters currently supported: median, dgaussian, bilateral. |

## Examples

`TeamSky.exe -h`<br />
Display help menu.<br />
<br />

`TeamSky.exe -3d ../../Images/test_set_1/ 25`<br />
Create a 3D volume from 25 images that are in the directory ../../Images/test_set_1 named as 000000.dcm through 000024.dcm<br />
<br />

`TeamSky.exe -f median`<br />
Since no filter arguments are provided, usage information for median filter will be displayed.<br />
<br />

`TeamSky.exe -f median image.dcm 2`<br />
Apply median filter with a radius of 2 to the image file, image.dcm.<br />
<br />

`TeamSky.exe -f dgaussian image.dcm 2`<br />
Apply discrete gaussian filter with a variance of 2 to the image file, image.dcm.<br />
<br />

`TeamSky.exe -f bilateral image.dcm 4 8`<br />
Apply bilateral filter with a domain sigma of 4 and a range sigma of 8 to the image file, image.dcm.<br />
<br />

`TeamSky.exe -s image.dcm 0.0025 0.25`<br />
Apply watershed segmentation to image.dcm with threshold set to 0.0025 and level set to 0.25. It is recommended to apply some smoothing filter to the image before applying watershed segmentation.<br />
<br />

`TeamSky.exe -s image.dcm 0.0025 0.25 1200`<br />
Apply watershed segmentation to image.dcm with threshold set to 0.0025 and level set to 0.25. It is recommended to apply some smoothing filter to the image before applying watershed segmentation. The last argument removed labels/segments that have a membership count less than or equal to 1200. Note: this value fluctuates with the size of the image. <br />
<br />

More options and examples to be added later.