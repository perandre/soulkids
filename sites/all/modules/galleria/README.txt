// $Id: README.txt,v 1.2.2.6 2009/05/20 16:28:15 marktheunissen Exp $

Galleria
---------------------

1. Install and enable Galleria.

2. Decide whether you want to attach your images using Drupal core's Upload module, or if you'd like to use the ImageField module. When Galleria was first written, ImageField was still in early alpha, so I developed for Upload module only. Now it supports both.

As a general rule, the fewer unnecessary modules you use on your Drupal site, the better. If you find that just using the Upload module rather than ImageField is fine for you, then rather do that. However, using ImageField has certain advantages, such as having thumbnails displayed on the node edit form.

If you want the entire Galleria to open in a Lightbox, then you have to use the Upload module to attach images.

3. Follow the guide below for your preferred choice. Note that you can swop at any time and even use both methods on the same site.

4. Configure general options at 'admin/settings/galleria'.

Upload Module
---------------------

Drupal core provides the Upload module. You attach your images to a node, and when you display it, Galleria module replaces the attachment list with a Galleria.

1. Choose the content type that you want converted into Gallerias. All nodes of this type will be converted. You'd usually make a type called "galleria".
2. On that content type's "edit content type" page, you'll have an option "Galleria from attachments" under the "Workflow settings".
3. In that same "Workflow settings" section, enable file uploads (Attachments).
4. Create a new Galleria node, and attach the images using the upload files function.
5. View the node, and voila, you have a Galleria!

ImageField Module
---------------------

Much the same as the Upload module, except the images are drawn from an ImageField.

1. Install ImageField module and FileField module (that it depends on).
2. Create or choose an existing content type that will display the Galleria.
3. Add an ImageField (called FileField in the "Data type" drop-down) to the content type in the "Manage fields" section.
4. Set your desired configuration as per standard ImageField settings.
5. Click the "Display fields" section for the content type.
6. Change the output formatter for "Full node" and/or "Teaser" to display "Galleria".
7. Create a node of this type, add images, and view!

NOTE: Do not switch on the "Galleria from attachments" setting (detailed in the previous section) for ImageField-based Galleria content types. If you do, you'll see an error message.

Lightbox support
---------------------

You can have your entire Galleria open inside a Lightbox, but only if you are using the Upload module as the attachment method. Start by downloading and installing the Lightbox2 module from http://drupal.org/project/lightbox2. Then use the following Galleria theme function somewhere on your site:

<?php print theme('galleria_lightbox_link', $nid, $width, $height, $text); ?>

This will give you an anchor link to the Galleria with node id "$nid". Clicking the link opens a lightbox containing the Galleria.

Example: assume you have a Galleria with a node id of 12. You would go to whereever you want your link to this Galleria to be, and insert the following code:

<?php print theme('galleria_lightbox_link', 12, 800, 600, 'Click here for the Gallery'); ?>

Clicking the link that is produced will give you an 800x600 lightbox with the contents of node 12's Galleria in it!

You can also have the main image open in a lightbox when it's clicked, instead of the default behaviour of going to the next image in the Galleria. To do this, access the Galleria settings page and choose the Lightbox module from the Lightbox settings section. You can also choose an ImageCache preset that will be used to display the image.

ImageCache support
---------------------

If you have the ImageCache module installed, you can choose the preset that is used to display the Galleria thumbnails and main image. You can do this on the Galleria settings page.

jCarousel support
---------------------

When you enable the jCarousel module, you will have the option of displaying the Galleria thumbnails inside a jCarousel. There are settings on the Galleria settings page.

Captions
---------------------

To add a caption to each image, you can set the "Description" of each file to the caption you want. This is done on the node edit page when you are using the Upload module to attach images.

Customisation
---------------------

There is a preprocess hook and template files for all your customisation needs. You can style the galleria by overriding the CSS.
