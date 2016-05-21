﻿<%@ Page Language="C#" AutoEventWireup="true" CodeFile="Terminal.aspx.cs" Inherits="Terminal" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
	<link type="text/css" href="Resources/css/bootstrap.min.css" rel="stylesheet"/>
	<link type="text/css" href="Resources/css/bootstrap-responsive.min.css" rel="stylesheet"/>
	<link type="text/css" href="Resources/css/theme.css" rel="stylesheet"/>
	<link type="text/css" href="Resources/css/font-awesome.css" rel="stylesheet"/>
	<link type="text/css" href='http://fonts.googleapis.com/css?family=Open+Sans:400italic,600italic,400,600' rel='stylesheet'/>
    <title>MTMS - Terminal <%= Context.Request.Params["id"].ToString() %></title>
</head>
<body>

    <div class="navbar navbar-fixed-top">
            <div class="navbar-inner">
                <div class="container">
                    <a class="btn btn-navbar" data-toggle="collapse" data-target=".navbar-inverse-collapse">
                        <i class="icon-reorder shaded"></i></a><a class="brand" href="index.aspx">MTMS </a>
                    <div class="nav-collapse collapse navbar-inverse-collapse">
                        <ul class="nav pull-right">
                            <li><a href="#">Logout </a></li>
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
                    <div class="span9">
                        <div class="btn-controls">
                            <div class="btn-box-row row-fluid">
                                <h1 class="btn-box big span12">
                                    <p class="text-muted">Terminal</p><b> <%= Context.Request.Params["id"].ToString() %> </b>
                                </h1>
                            </div>
                            
                        </div>

                        <div class="btn-controls">
                            
							<div class="btn-box-row row-fluid">
								<a class="btn-box big span3">
                                    <input type="checkbox" />
									<i class="icon-adjust"></i>
									<b>Test Health</b>
								</a>
								<a class="btn-box big span3">
                                    <input type="checkbox" />
									<i class="icon-briefcase"></i>
									<b>List Files</b>
								</a>
								<a class="btn-box big span3">
                                    <input type="checkbox" />
									<i class="icon-gift"></i>
									<b>List Apps</b>
								</a>
                                <a class="btn-box big span3">
                                    <input type="checkbox" />
									<i class="icon-gift"></i>
									<b>Delete File</b>
								</a>
							</div>

							<div class="btn-box-row row-fluid">
							    <a class="btn-box big span4">
                                    <input type="checkbox" id="update_hotlist" />
									<i class="icon-adjust"></i>
									<b>Push File</b>
								</a>
								<a class="btn-box big span4">
                                    <input type="checkbox" />
									<i class="icon-briefcase"></i>
									<b>Pull File</b>
								</a>
								<a class="btn-box big span4">
                                    <input type="checkbox" />
									<i class="icon-gift"></i>
									<b>Update App</b>
								</a>
							</div>
                        </div>

                        <div class="module">
                            <div class="module-head">
                                <h3>Inputs</h3>
                            </div>
                            <div class="module-body" id="inputs_div">
                                <div class="align-right">
                                    <button type="submit" class="btn btn-large">Submit</button>
                                </div>
							</div>
                        </div>
                        <!--/.content-->
                    </div>
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
