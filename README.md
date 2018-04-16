# CSC621_TeamSky

## Description

This application applies ITK image processing algorithms.<br />

## Usage

Run application from Build/Debug/TeamSky.exe [args]<br />

Possible args are:<br />

| Option | Description |
| --- | --- |
| \-h, \-\-help | Display help menu |
| \-r | Registration |
| \-s | Segmentation |
| \-3d | Create 3D image from multiple 2D images<br />
**Usage**: TeamSky.exe \-3d image_dir number_of_images<br />
Where image_dir is a directory containing images named like 000000.dcm and number_of_images is the number of 2D images that will be used to create the 3D image. |

## Examples

TeamSky.exe -h<br />
Display help menu.<br />
<br />

TeamSky.exe -3d ../../Images/test_set_1/ 25<br />
Create a 3D volume from 25 images that are in the directory ../../Images/test_set_1 named as 000000.dcm through 000024.dcm<br />
<br />

More options and examples to be added later.