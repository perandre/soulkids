// $Id: imagebrowser.js,v 1.1.2.12 2009/07/19 14:30:51 starnox Exp $

/**
 * @file imagebrowser.js
 *
 * All the javascript to make Image Browser work
 */

/**
 * FCKEditor insert function
 */
function SelectFile(url, width, height, alt, link, link_target, styles)
{
  if(window.opener) {
    //url, width, height, alt
    window.opener.SetUrl( url, null, null, alt );

    if (window.opener.GetE('txtLnkUrl')) {
      window.opener.GetE('txtLnkUrl').value = link ;
    }

    if (window.opener.GetE('cmbLnkTarget')) {
      window.opener.GetE('cmbLnkTarget').value = link_target ;
    }

    if (window.opener.GetE('txtAttClasses')) {
      if(styles == '') {
        window.opener.GetE('txtAttClasses').value = 'ibimage';
      }
      else {
        window.opener.GetE('txtAttClasses').value = 'ibimage ' + styles;
      }
    }
  }
  window.close();
}

/**
 * Prepare links in the browser for insert overlay
 */
function ib_prepareLinks() {
  //Add on click functions for images
  $("#browse .browse .view-content ul li a").click(function() {
    $("#fade").show();
    $("#fade").fadeTo('fast', 0.7);
    $("#insert").fadeIn('fast');
    $("#insert").html('<p class="loading"></p>');
    var href = $(this).attr("href");
    var rand = Math.random();
    $("#insert").load(href, function() {
      //Cancel button
      $(".close").click(function() {
        $("#fade").fadeOut('slow');
        $("#insert").fadeOut('slow');
        return false;
      });
      
      //Insert button
      $(".insert").click(function() {
        var url = $("#ib_image_preset").val();
        var alt = 'imagebrowser image';
        if($("#ib_link_options").val() == 'custom') {
          var link = $("#ib_link_custom").val();
        }
        else {
          var link = $("#ib_link_options").val();
        }
        var link_target = $("#ib_link_target").val();
        
        var styles = $("#ib_alignment").val();
        var alt = $("#ib_image_alt").val();
        
        SelectFile(url, null, null, alt, link, link_target, styles);
        return false;
      });
      
      //Edit button
      $(".edit").click(function(){
        $("#edit").fadeIn('fast');
        var href = $(this).attr("href");
        $("#edit").html('<p class="loading"></p>');
        var href = $(this).attr("href");
        $("#edit").load(href, function() {
          var src = $("#edit > iframe").attr("src");
          $("#edit > iframe").attr("src", src);
          //Cancel button
          $(".close").click(function() {
            $(".view-filters > form").trigger('submit');
            $("#insert").fadeOut();
            $("#fade").fadeOut();
            $("#edit").fadeOut();
            return false;
          });
        });
        return false;
      });
      
      //Delete button
      $(".delete").click(function(){
        var node = $(this).attr("href");
        $("#insert > .details > .options a").fadeOut('slow');
        $("#insert > .details > table").slideUp('slow', function() {
          $("#insert > .details > .confirm").fadeIn('fast');
          $(".confirm").fadeIn('fast');
          
          $(".cancel").click(function() {
            $(".confirm").fadeOut('fast', function() {
              $("#insert > .details > .confirm").fadeOut('fast');
              $("#insert > .details > .options a").fadeIn('slow');
              $("#insert > .details > table").slideDown('slow');
            });
          });
          
          $(".delete_confirm").click(function() {
            $("#insert > .details").append('<div class="delete"></div>');
            $(".delete").load(node, function() {
              $(".view-filters > form").trigger('submit');
              $("#insert").fadeOut();
              $("#fade").fadeOut();
              ib_get_messages();
            });
          });
        });
        return false;
      });
      
      //Link Selection
      $("#ib_link_options").change(function() {
        if($("#ib_link_options").val() == 'custom') {
          $("#ib_link_custom").css('display', 'block');
        }
        else if($("#ib_link_custom").css("display") == 'block') {
          $("#ib_link_custom").css('display', 'none');
        }
        
        if($("#ib_link_options").val() == '') {
          $("#ib_link_target").css('display', 'none');
        }
        else {
          $("#ib_link_target").css('display', 'block');
        }
      });
    });
    return false;
  });

  //Add on click to apply filters button
  $("#edit-submit").click(function(){
    $("#fade").fadeOut("slow");
    $(".view-filters").fadeOut("slow");
  });
  
  //Format filter window
  ib_format_filters();
  
  //Button to close all windows
  $(".footer .apply").click(function(){
    $("#fade").fadeOut('slow');
    $("#messages").fadeOut('slow');
    $(".view-filters").fadeOut('slow');
  });
  $(".footer .close").click(function(){
    $("#fade").fadeOut('slow');
    $("#messages").fadeOut('slow');
    $(".view-filters").fadeOut('slow');
    return false;
  });
}

/**
 * Get Drupal messages
 */
function ib_get_messages() {
  var pre_num = $(".open-messages .num").text();
  var num = 0;
  $("#messages > .log").prepend('<li class="hidden"></li>');
  $("#messages > .log > li:first-child").load(ibpath, function() {
    if ( $("#messages > .log > li:first-child").children().length > 0) {
      $("#messages > .log > li:first-child").removeClass('hidden');
    }
    
    num = $("#messages > .log").children().not(".hidden").length;
    
    //Flash button
    if (parseInt(pre_num) < parseInt(num)) {
      $(".open-messages").animate({opacity: "0"}, 250, function() {
        $(".open-messages .num").text(num);
        $(".open-messages").animate({opacity: "1"}, 250);
      });
    }
  });
}

/**
 * Since the filters window is made by Views we can pretty it up via JS
 */
function ib_format_filters() {
  $(".views-exposed-widgets > div:last-child").appendTo("#views-exposed-form-ib-browser-default").addClass("footer");
  $("#views-exposed-form-ib-browser-default > div:first-child").addClass("wrapper");
  $("#views-exposed-form-ib-browser-default h2").prependTo("#views-exposed-form-ib-browser-default");
  $("#views-exposed-form-ib-browser-default > .footer").click(function(){
    $(".view-filters > form").trigger('submit');
  })
}

/**
 * After an image upload refresh the view and display any messages
 */
function ib_refresh() {
  //Display post upload messages
  ib_get_messages();
  //Trigger exposed filters submit to refresh view
  $(".view-filters > form").trigger('submit');
}

/**
 * When they pick an image upload the file right away
 */
function ib_upload() {
  $("#edit-attach").trigger('mousedown');
}

/**
 * Make things happen on page load
 */
Drupal.behaviors.imagebrowser = function(context) {
  ib_prepareLinks();

  //Button to open the Filter window
  $(".open-filters").click(function(){
    $("#fade").show();
    $("#fade").fadeTo('fast', 0.7);
    $(".view-filters").fadeIn('fast');
  });

  //Button to open the Messages window
  $(".open-messages").click(function(){
    ib_get_messages();
    $("#fade").show();
    $("#fade").fadeTo('fast', 0.7);
    $("#messages").fadeIn('fast');
  });

  //Delay display until JS has loaded
  $("#fade").fadeOut('slow', function() {
    $("#fade").css('background', '#000000');
  });
}