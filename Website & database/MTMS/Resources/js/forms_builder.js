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
    $("#DELETE_FILE").click(function () {
        if ($("#DELETE_FILE").is(':checked')) {
            $("#delete_file_params").show();
        }
        else {
            $("#delete_file_params").hide();
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
            if ($("#update_hotlist").get(0).files.length == 0 && $("#update_config").get(0).files.length == 0) {
                alert("Please choose hotlist/config file to update!");
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
    });

    $("#SelectAllGroups").change(function () {
        $('#groups input:checkbox').each(function() {
            $(this).prop('checked', $("#SelectAllGroups").is(':checked'));
        });
    });
    $("#SelectAllTerminals").change(function () {
        $('#terminals input:checkbox').each(function () {
            $(this).prop('checked', $("#SelectAllTerminals").is(':checked'));
        });
    });
    $("#SelectAllOuters").change(function () {
        $('#outers_table input:checkbox').each(function () {
            $(this).prop('checked', $("#SelectAllOuters").is(':checked'));
        });
    });
    $("#SelectAllFiles").change(function () {
        $('#history input:checkbox').each(function () {
            $(this).prop('checked', $("#SelectAllFiles").is(':checked'));
        });
    });
    $(".treeview").delegate("label input:checkbox", "change", function () {
        var
            checkbox = $(this),
            nestedList = checkbox.parent().next().next(),
            selectNestedListCheckbox = nestedList.find("label:not([for]) input:checkbox");

        if (checkbox.is(":checked")) {
            return selectNestedListCheckbox.prop("checked", true);
        }
        selectNestedListCheckbox.prop("checked", false);
    });
    $(".treeview-nocheckbox").delegate("label input:checkbox", "change", function () {
        var
            checkbox = $(this),
            nestedList = checkbox.parent().next().next(),
            selectNestedListCheckbox = nestedList.find("label:not([for]) input:checkbox");

        if (checkbox.is(":checked")) {
            return selectNestedListCheckbox.prop("checked", true);
        }
        selectNestedListCheckbox.prop("checked", false);
    });
});