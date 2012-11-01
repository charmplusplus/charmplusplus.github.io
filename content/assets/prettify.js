$(document).ready( function() {
    // Grab all code snippets and paint it to highlight syntax
    $("pre code").each( function(idx) {
        CodeMirror.runMode($(this).text(), "text/x-charm++", $(this).get(0));
    })
    .addClass("cm-s-lesser-dark")
    .children("span.cm-charmkeyword").css("color", "#dd5ef3");

    // Enable top pull-down menu functionality
    $(".navigation")
    .append('<span id="pulldowntab" class="navsymbol">&raquo;</span>')
    .click( function() { $("ul.manual-toc").fadeToggle(); $("#pulldowntab").toggle(); } )
    .mouseleave( function() { $("ul.manual-toc").fadeOut('slow'); $("#pulldowntab").fadeIn('slow'); } )
    .css('cursor','pointer');

    // Enable floating navigation menu functionality
	$("#nav-floatmenu > li")
	.mouseenter( function() { $(this).css("border-color", "#00a4e4").children('div').show(); } )
	.mouseleave( function() { $(this).css("border-color", "#ffffff").children('div').hide(); } );

} )

