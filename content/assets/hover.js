//Add Hover effect to menus
jQuery('ul.nav li.dropdown').hover(function() {
    jQuery(this).addClass("open");
    jQuery(this).find('.dropdown-menu').stop(true, true).fadeIn();
}, function() {
    jQuery(this).removeClass("open");
    jQuery(this).find('.dropdown-menu').stop(true, true).fadeOut();
});
