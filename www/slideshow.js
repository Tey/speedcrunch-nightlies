// Copyright (C) 2014 Helder Correia [http://heldercorreia.net]

var TOTAL_SLIDES = 6;
var g_currentSlide = -1;

function activateSlide() {
    var style = document.getElementById("slide").style;
    style.opacity = "0.20";
    style.transform = "scale(1.2)";
    style.webkitTransform = "scale(1.2)";

    setTimeout(function () {
        hideSlide();
    }, 6000);
}

function restoreScale() {
    var style = document.getElementById("slide").style;
    style.transition = "";
    style.webkitTransition = "";
    style.transform = "scale(1)";
    style.webkitTransform = "scale(1)";

    setTimeout(function () {
        showNextSlide();
    }, 0);
}

function hideSlide() {
    var style = document.getElementById("slide").style;
    style.opacity = "0";

    setTimeout(function () {
        restoreScale();
    }, 3500);
}

function showNextSlide() {
    if (g_currentSlide === TOTAL_SLIDES)
        g_currentSlide = 0;
    else
        ++g_currentSlide;

    var slide = document.getElementById("slide");
    slide.parentNode.removeChild(slide);
    slide = document.createElement('div');
    slide.id = "slide";
    slide.style.zIndex = "-1000";

    var style = slide.style;
    style.opacity = "0";
    style.width = "100%";
    style.height = "100%";
    style.position = "fixed";
    style.left = "0";
    style.top = "0";
    style.backgroundImage = "url(images/slide" + g_currentSlide + ".png)";
    style.backgroundRepeat = "no-repeat";
    style.webkitTransition = "opacity 3s linear, -webkit-transform 10s linear";
    style.transition = "opacity 3s linear, transform 10s linear";

    if (g_currentSlide === 0) {
        style.backgroundSize = "contain";
        style.backgroundPosition = "center center";
    } else {
        style.backgroundSize = "cover";
        style.backgroundPosition = "left bottom";
    }

    document.body.appendChild(slide);

    setTimeout(function() {
        activateSlide();
    }, 0);
}

function checkBrowser() {
    if (document.getElementById("noIE11") !== null)
        alert("Please upgrade to Microsoft Internet Explorer 11 for this website to be properly rendered. Or consider using Google Chrome, Mozilla Firefox, or Opera.");
}

function main() {
    checkBrowser();
    setTimeout("showNextSlide();", 0);
    setTimeout(function() {
        document.getElementById("menu").className = "full-opaque";
    }, 2000);
}
