<%@ Page Language="C#" AutoEventWireup="true" CodeFile="Groups.aspx.cs" Inherits="groups" %>

<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
    <link rel="shortcut icon" type="image/x-icon" href="Resources/img/favicon.ico" />
	<link type="text/css" href="Resources/css/bootstrap.min.css" rel="stylesheet"/>
	<link type="text/css" href="Resources/css/bootstrap-responsive.min.css" rel="stylesheet"/>
	<link type="text/css" href="Resources/css/theme.css" rel="stylesheet"/>
	<link type="text/css" href="Resources/css/font-awesome.css" rel="stylesheet"/>
    <link type="text/css" href="Resources/css/font-awesome43.css" rel="stylesheet"/>
	<link type="text/css" href='http://fonts.googleapis.com/css?family=Open+Sans:400italic,600italic,400,600' rel='stylesheet'/>
    <title>MTMS - Groups</title>
</head>
<body>

    <div class="navbar navbar-fixed-top">
            <div class="navbar-inner">
                <div class="container">
                    <a class="btn btn-navbar" data-toggle="collapse" data-target=".navbar-inverse-collapse">
                        <i class="icon-reorder shaded"></i></a><a class="brand" href="index.aspx">MTMS </a>
                    <div class="nav-collapse collapse navbar-inverse-collapse">
                        <ul class="nav pull-right">
                            <li><a href="logout.aspx">Logout </a></li>
                        </ul>
                    </div>
                    <!-- /.nav-collapse -->
                </div>
            </div>
            <!-- /navbar-inner -->
        </div>
        <!-- /navbar -->
        <div class="wrapper">
            <div class="container">
                <div class="row">
                    <div class="span3">
                        <div class="sidebar">
                            <ul class="widget widget-menu unstyled">
                                <li><a href="groups.aspx"><i class="menu-icon icon-group"></i>Groups <b class="label green pull-right"> <%= Methods.getGroupsCount() %> </b></a></li>
                                <li><a href="terminals.aspx"><i class="menu-icon icon-hdd"></i>Terminals<b class="label orange pull-right"> <%= Methods.getTerminalsCount() %> </b></a></li>
                                <li><a href="config.aspx"><i class="menu-icon icon-hdd"></i>Generate Config File<b class="label orange pull-right"></b></a></li>
                            </ul>
                            <!--/.widget-nav-->
                        </div>
                        <!--/.sidebar-->
                    </div>
                    <!--/.span3-->
                    <form runat="server" class="span9" id="GroupsForm">
                        <% PrintStatus(); %>
                        <div class="content">
                            <div class="module">
                                <div class="module-head">
                                    <h3>
                                        Groups</h3>
                                </div>
                                <div class="module-body table">
                                    <table id="groups" border="0" class="datatable-1 table table-bordered table-striped  display dataTable"  aria-describedby="DataTables_Table_0_info" style="width: 100%;">
								        <thead>
									        <tr>
                                                <th><div class="inline-block-center"><asp:CheckBox runat="server" ID="SelectAllGroups" CssClass="checkbox-without-label"/><//div></th>
                                                <th rowspan="1" colspan="1">Id</th>
                                                <th rowspan="1" colspan="1">Group Name</th>
                                                <th rowspan="1" colspan="1">Branch</th>
                                                <th rowspan="1" colspan="1">Contact Number</th>
                                                <th rowspan="1" colspan="1">Contact Person</th>
                                                <th rowspan="1" colspan="1">Date Added</th>
                                                <th rowspan="1" colspan="1">POS Count</th>
									        </tr>
								        </thead>
								        <tbody role="alert" aria-live="polite" aria-relevant="all">
                                            <%--<% PrintGroups(); %>--%>
                                            <asp:Repeater runat="server" ID="GroupsRep">
                                                <ItemTemplate>
                                                    <tr>
                                                        <td><div class="inline-block-center"><asp:CheckBox runat="server" CssClass="checkbox-without-label"/></div></td>
                                                        <td><a href="group.aspx?id=<%# Eval("Id") %>&name=<%# Eval("Name") %>"> <%# Eval("Id") %> </a></td>
                                                        <td> <%# Eval("Name") %> </td>
                                                        <td> <%# Eval("Branch") %> </td>
                                                        <td> <%# Eval("ContactNumber") %> </td>
                                                        <td> <%# Eval("ContactPerson") %> </td>
                                                        <td> <%# Eval("DateCreated") %> </td>
                                                        <td> <%# Eval("TerminalsCount") %> </td>
                                                    </tr>
                                                </ItemTemplate>
                                            </asp:Repeater>
								        </tbody>
                                    </table>
							    </div>
                                <div class="module-body">
                                    <div class="align-right">
                                        <asp:Button ID="FreezeGroupBtn" CausesValidation="false" runat="server" CssClass="btn btn-primary btn-large" Text="Freeze Terminals" OnClick="FreezeGroupBtn_Click"/>
                                        <asp:Button ID="DeleteGroupBtn" CausesValidation="false" runat="server" CssClass="btn btn-primary btn-large" Text="Delete" OnClick="DeleteGroupBtn_Click"/>
                                    </div>
                                </div>
                            </div>
                        </div>
                        <div class="module" id="groups_module">
                            <div class="module-head">
                                <h3>Create New Group</h3>
                            </div>
                            <div class="module-body" runat="server">
                                <div class="control-group">
									<div class="controls">
										<asp:TextBox runat="server" id="NewNameTxt" placeholder="Group Name" CssClass="span3" />
										<asp:RequiredFieldValidator runat="server" ErrorMessage="*" ControlToValidate="NewNameTxt" ForeColor="Red" ValidationGroup="CreateGroup" />
                                    </div>
                                    <div class="controls">
										<asp:TextBox runat="server" id="NewBranchTxt" placeholder="Branch" CssClass="span3" />
                                        <asp:RequiredFieldValidator runat="server" ErrorMessage="*" ControlToValidate="NewBranchTxt" ForeColor="Red" ValidationGroup="CreateGroup" />
                                    </div>
                                    <div class="controls">
										<asp:TextBox runat="server" id="ContactNumTxt" placeholder="Contact Number" CssClass="span3" />
										<asp:RequiredFieldValidator runat="server" ErrorMessage="*" ControlToValidate="ContactNumTxt" ForeColor="Red" ValidationGroup="CreateGroup" />
                                        <asp:RegularExpressionValidator ID="ContactNumValidator" runat="server" ErrorMessage="Invalid Phone Number"
                                            ValidationExpression="^01[0-2]{1}[0-9]{8}"
                                            ControlToValidate="ContactNumTxt"
                                            ForeColor="Red" ValidationGroup="CreateGroup" />
                                    </div>
                                    <div class="controls">
										<asp:TextBox runat="server" id="ContactPersonTxt" placeholder="Contact Person" CssClass="span3" />
										<span class="help-inline">Leave empty if not specified</span>
									</div>
                                    <div class="align-right">
                                        <asp:Button ID="CreateGroupBtn" runat="server" CssClass="btn btn-primary btn-large" Text="Create" OnClick="CreateGroupBtn_Click" ValidationGroup="CreateGroup"></asp:Button>
                                    </div>
								</div>
							</div>
                        </div>
                        <div class="module">
                            <div class="module-head">
                                    <h3>
                                        Ongoing Group Command</h3>
                            </div>
                        </div>
                        <div class="btn-controls">
                            
							<div class="btn-box-row row-fluid">
								<a class="btn-box big span4">
                                    <asp:CheckBox id="TEST_HEALTH" runat="server" />
                                    <label class="checkbox-label" for="TEST_HEALTH"><i class="fa fa-heartbeat"></i><br /><span class="checkbox-with-label"></span>Test Health</label>
								</a>
								<a class="btn-box big span4">
                                    <asp:CheckBox id="UPDATE_APP" runat="server"/>
                                    <label class="checkbox-label" for="UPDATE_APP"><i class="fa fa-angle-double-up"></i><br /><span class="checkbox-with-label"></span>Update App</label>
								</a>
								<a class="btn-box big span4">
                                    <asp:CheckBox id="LIST_FILES" runat="server"/>
                                    <label class="checkbox-label" for="LIST_FILES"><i class="fa fa-file-text"></i><br /><span class="checkbox-with-label"></span>List Files</label>
								</a>
							</div>

							<div class="btn-box-row row-fluid">
							    <a class="btn-box big span4">
                                    <asp:CheckBox id="PULL_FILE" runat="server"/>
                                    <label class="checkbox-label" for="PULL_FILE"><i class="fa fa-download"></i><br /><span class="checkbox-with-label"></span>Update Hotlist/Config</label>
								</a>
								<a class="btn-box big span4">
                                    <asp:CheckBox id="PUSH_FILE" runat="server"/>
                                    <label class="checkbox-label" for="PUSH_FILE"><i class="fa fa-upload"></i><br /><span class="checkbox-with-label"></span>Get Transaction File</label>
								</a>
								<a class="btn-box big span4">
                                    <asp:CheckBox id="LIST_APPS" runat="server"/>
									<label class="checkbox-label" for="LIST_APPS"><i class="fa fa-list"></i><br /><span class="checkbox-with-label"></span>List Apps</label>
								</a>
							</div>
                        </div>

                        <div class="module" id="group_module" runat="server">
                            <div class="module-head">
                                <h3>Inputs</h3>
                            </div>
                            <div class="module-body" id="inputs_div" runat="server">
                                <div id='update_app_params' hidden="hidden">
                                    <h4>Update App parameters</h4>
                                    <label>CAB File:</label>
                                    <asp:FileUpload AllowMultiple="true" id='update_app_cab' runat="server" />
                                    <asp:RegularExpressionValidator ID="CabValidator" runat="server" ErrorMessage="Invalid .cab source"
                                            ValidationExpression="(.+\.([Cc][Aa][Bb]))"
                                            ControlToValidate="update_app_cab"
                                            ForeColor="Red" />
                                    <br/>
                                    <label>APK File:</label>
                                    <asp:FileUpload AllowMultiple="true" id='update_app_apk' runat="server" />
                                    <asp:RegularExpressionValidator ID="ApkValidator" runat="server" ErrorMessage="Invalid .apk source"
                                            ValidationExpression="(.+\.([Aa][Pp][Kk]))"
                                            ControlToValidate="update_app_apk"
                                            ForeColor="Red"/>
                                    <hr/>
                                </div>
                                <div id='update_hotlist_params' hidden="hidden">
                                    <h4>Update Hotlist parameters</h4>
                                    <label>Hotlist File:</label>
                                    <asp:FileUpload id='update_hotlist' runat="server" />
                                    <label>Config File:</label>
                                    <asp:FileUpload id='update_config' runat="server" />
                                    <hr/>
                                </div>
                                <div class="align-right">
                                    <asp:CheckBox id="Schedule" runat="server"/>
                                    <label for="Schedule"><span class="checkbox-with-label"></span>Schedule this command</label>
                                    <asp:Button ID="GroupSubmitBtn" runat="server" CssClass="btn btn-primary btn-large" Text="Submit" OnClick="GroupSubmitBtn_Click" ></asp:Button>
                                    <asp:Button ID="GroupClearBtn" runat="server" CssClass="btn btn-large" Text="Clear" OnClick="GroupClearBtn_Click"></asp:Button>
                                </div>
							</div>
                        </div>
                        <!--/.content-->
                    </form>
                    <!--/.span9-->
                </div>
            </div>
            <!--/.container-->
        </div>
        <!--/.wrapper-->
        <div class="footer">
            <div class="container">
                <b class="copyright">&copy; 2016 MTMS</b>
            </div>
        </div>

	<script src="Resources/js/jquery-1.9.1.min.js" type="text/javascript"></script>
	<script src="Resources/js/jquery-ui-1.10.1.custom.min.js" type="text/javascript"></script>
	<script src="Resources/js/bootstrap.min.js" type="text/javascript"></script>
    <script src="Resources/js/jquery.dataTables.js" type="text/javascript"></script>
    <script src="Resources/js/forms_builder.js" type="text/javascript"></script>
    <script>
        $(document).ready(function () {
            $('#groups').DataTable();
            $('.dataTables_paginate').addClass("btn-group datatable-pagination");
            $('.dataTables_paginate > a').wrapInner('<span />');
            $('.dataTables_paginate > a:first-child').append('<i class="icon-chevron-left shaded"></i>');
            $('.dataTables_paginate > a:last-child').append('<i class="icon-chevron-right shaded"></i>');
        });
    </script>

</body>
</html>
