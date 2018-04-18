# CSC621_TeamSky

## Description

This application applies ITK image processing algorithms.<br />

## Usage

Run application from Build/Debug/TeamSky.exe [args]<br />
Currently options must be written in order.<br />

Possible args are:<br />

| Option | Description |
| --- | --- |
| \-h, \-\-help | Display help menu |
| \-r | Registration |
| \-s | Segmentation |
| \-3d image_dir num | Create 3D image from multiple 2D images. image_dir is a directory containing images named like 000000.dcm. num is the number of images you want to use in that directory from 000000.dcm through 00000N.dcm |
| \-f filter_name [args] | Apply generic filter to image. Optional args may be required by different filter types. Filters currently supported: median |

## Examples

`TeamSky.exe -h`<br />
Display help menu.<br />
<br />

`TeamSky.exe -3d ../../Images/test_set_1/ 25`<br />
Create a 3D volume from 25 images that are in the directory ../../Images/test_set_1 named as 000000.dcm through 000024.dcm<br />
<br />

`TeamSky.exe -f median image.dcm 2`<br />
Apply median filter with a radius of 2 to the image file, image.dcm.<br />
<br />

More options and examples to be added later.