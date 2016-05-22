$(document).ready(function () {
    $("#UPDATE_APP").click(function () {
        if ($("#UPDATE_APP").is(':checked')) {
            $("#update_app_params").show();
        }
        else {
            $("#update_app_params").hide();
        }
    });
    $("#PULL_FILE").click(function () {
        if ($("#PULL_FILE").is(':checked')) {
            $("#update_hotlist_params").show();
        }
        else {
            $("#update_hotlist_params").hide();
        }
    });
    $("#SubmitBtn").click(function (event) {
        if ($("#UPDATE_APP").is(':checked')) {
            if ($("#update_app_apk").get(0).files.length == 0 && $("#update_app_cab").get(0).files.length == 0) {
                alert("Please choose app .apk or .cab to update!");
                event.preventDefault();
            }
        }
        if ($("#PULL_FILE").is(':checked')) {
            if ($("#update_hotlist").get(0).files.length == 0) {
                alert("Please choose hotlist file to update!");
                event.preventDefault();
            }
        }
    });
});