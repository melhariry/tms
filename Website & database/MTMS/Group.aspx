<%@ Page Language="C#" AutoEventWireup="true" CodeFile="Group.aspx.cs" Inherits="Group" %>

<!DOCTYPE html>

<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
	<link type="text/css" href="Resources/css/bootstrap.min.css" rel="stylesheet"/>
	<link type="text/css" href="Resources/css/bootstrap-responsive.min.css" rel="stylesheet"/>
	<link type="text/css" href="Resources/css/theme.css" rel="stylesheet"/>
	<link type="text/css" href="Resources/css/font-awesome.css" rel="stylesheet"/>
	<link type="text/css" href='http://fonts.googleapis.com/css?family=Open+Sans:400italic,600italic,400,600' rel='stylesheet'/>
    <title>MTMS - Group <%= Context.Request.Params["name"].ToString() %></title>
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
                            </ul>
                            <!--/.widget-nav-->
                        </div>
                        <!--/.sidebar-->
                    </div>
                    <!--/.span3-->
                    <form class="span9" runat="server" id="GroupForm">
                        <div class="btn-controls">
                            <div class="btn-box-row row-fluid">
                                <h1 class="btn-box big span12">
                                    <p class="text-muted">
                                        Group</p><b> <%= Context.Request.Params["name"].ToString() %> </b>
                                </h1>
                            </div>
                            
                        </div>
                        <% PrintStatus(); %>
                        <div class="content">
                            
                            <div class="module">
                                <div class="module-head">
                                    <h3>
                                        Terminals</h3>
                                </div>
                                <div class="module-body table">
                                    <table id="terminals" border="0" class="datatable-1 table table-bordered table-striped  display dataTable"  aria-describedby="DataTables_Table_0_info" style="width: 100%;">
								        <thead>
									    <tr>
                                            <th rowspan="1" colspan="1">Id</th>
                                            <th rowspan="1" colspan="1">Serial Number</th>
                                            <th rowspan="1" colspan="1">Vendor</th>
                                            <th rowspan="1" colspan="1">Model</th>
                                            <th rowspan="1" colspan="1">Last Online</th>
                                            <th rowspan="1" colspan="1">Last Ip</th>
                                            <th rowspan="1" colspan="1">Date Added</th>
									    </tr>
								    </thead>
								        <tbody role="alert" aria-live="polite" aria-relevant="all">
                                            <% PrintGroupTerminals(); %>
								        </tbody>
                                    </table>
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
									<i class="icon-adjust"></i>
									<b>Test Health</b>
								</a>
								<a class="btn-box big span4">
                                    <asp:CheckBox id="UPDATE_APP" runat="server"/>
									<i class="icon-briefcase"></i>
									<b>Update App</b>
								</a>
								<a class="btn-box big span4">
                                    <asp:CheckBox id="LIST_FILES" runat="server"/>
									<i class="icon-gift"></i>
									<b>List Files</b>
								</a>
							</div>

							<div class="btn-box-row row-fluid">
							    <a class="btn-box big span4">
                                    <asp:CheckBox id="PULL_FILE" runat="server"/>
									<i class="icon-adjust"></i>
									<b>Update Hotlist</b>
								</a>
								<a class="btn-box big span4">
                                    <asp:CheckBox id="PUSH_FILE" runat="server"/>
									<i class="icon-briefcase"></i>
									<b>Get Transaction File </b>
								</a>
								<a class="btn-box big span4">
                                    <asp:CheckBox id="LIST_APPS" runat="server"/>
									<i class="icon-gift"></i>
									<b>List Apps</b>
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
                                    <asp:FileUpload id='update_app_cab' runat="server" /><br/>
                                    <label>APK File:</label>
                                    <asp:FileUpload id='update_app_apk' runat="server" />
                                    <hr/>
                                </div>
                                <div id='update_hotlist_params' hidden="hidden">
                                    <h4>Update Hotlist parameters</h4>
                                    <label>Hotlist File:</label>
                                    <asp:FileUpload id='update_hotlist' runat="server" />
                                    <hr/>
                                </div>
                                <div class="align-right">
                                    <asp:Button ID="GroupSubmitBtn" runat="server" CssClass="btn btn-large" Text="Submit" OnClick="GroupSubmitBtn_Click"></asp:Button>
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
            $('#terminals').DataTable();
            $('.dataTables_paginate').addClass("btn-group datatable-pagination");
            $('.dataTables_paginate > a').wrapInner('<span />');
            $('.dataTables_paginate > a:first-child').append('<i class="icon-chevron-left shaded"></i>');
            $('.dataTables_paginate > a:last-child').append('<i class="icon-chevron-right shaded"></i>');
        });
    </script>

</body>
</html>

