$(document).ready(function () {
    $("#update_app").click(function () {
        if ($("#update_app").is(':checked')) {
            $("#inputs_div").append("<div id='update_app_params'><h4>Update App parameters</h4><label>CAB File:</label><input id='update_app_cab' type='file' accept='.cab' /><br><label>APK File:</label><input id='update_app_apk' type='file' accept='.apk' /><hr></div");
        }
        else {
            $("#update_app_params").remove();
        }
    });
    $("#update_hotlist").click(function () {
        if ($("#update_hotlist").is(':checked')) {
            $("#inputs_div").append("<div id='update_hotlist_params'><h4>Update Hotlist parameters</h4><label>Hotlist File:</label><input id='update_hotlist' type='file' accept='.txt' /><hr></div");
        }
        else {
            $("#update_hotlist_params").remove();
        }
    });
    
});