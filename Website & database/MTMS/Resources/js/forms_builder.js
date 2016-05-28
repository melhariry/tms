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
            $("#update_hotlist_params, #pull_file_params").show();
        }
        else {
            $("#update_hotlist_params, #pull_file_params").hide();
        }
    });
    $("#PUSH_FILE").click(function () {
        if ($("#PUSH_FILE").is(':checked')) {
            $("#push_file_params").show();
        }
        else {
            $("#push_file_params").hide();
        }
    });
    $("#GroupSubmitBtn").click(function (event) {
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
    $("#TerminalSubmitBtn").click(function (event) {
        if ($("#UPDATE_APP").is(':checked')) {
            if ($("#update_app_src").get(0).files.length == 0) {
                alert("Please choose app src to update!");
                event.preventDefault();
            }
        }
        if ($("#PULL_FILE").is(':checked')) {
            if ($("#pull_file_src").get(0).files.length == 0) {
                alert("Please choose pull file parameter!");
                event.preventDefault();
            }
        }
        if ($("#PUSH_FILE").is(':checked')) {
            if (!$("#hotlist_file").is(':checked') && !$("#tms_conf").is(':checked') && !$("#trsc_conf").is(':checked')) {
                alert("Please pick at least one file to be pushed!");
                event.preventDefault();
            }
        }
    });
    $("#CreateGroupBtn").click(function (event) {
        var inputtxt = $("ContactNumTxt").val();
        var phoneno = /^\(?([0-9]{3})\)?[-. ]?([0-9]{3})[-. ]?([0-9]{4})$/;
        if (inputtxt.value.match(phoneno)) {
            alert("Invalid phone number format");
            event.preventDefault();
        }
    });
});