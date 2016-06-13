<%@ Page Language="C#" AutoEventWireup="true" CodeFile="Index.aspx.cs" Inherits="Index" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
	<meta name="viewport" content="width=device-width, initial-scale=1.0"/>
    <link rel="shortcut icon" type="image/x-icon" href="Resources/img/favicon.ico" />
	<link type="text/css" href="Resources/css/bootstrap.min.css" rel="stylesheet"/>
	<link type="text/css" href="Resources/css/bootstrap-responsive.min.css" rel="stylesheet"/>
	<link type="text/css" href="Resources/css/theme.css" rel="stylesheet"/>
	<link type="text/css" href="Resources/css/font-awesome.css" rel="stylesheet"/>
	<link type="text/css" href='http://fonts.googleapis.com/css?family=Open+Sans:400italic,600italic,400,600' rel='stylesheet'/>
    <title>MTMS - Dashboard</title>
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
                                <li><a href="config.aspx"><i class="menu-icon icon-cog"></i>Configuration<b class="label orange pull-right"></b></a></li>
                            </ul>
                            <!--/.widget-nav-->
                        </div>
                        <!--/.sidebar-->
                    </div>
                    <!--/.span3-->
                    <div class="span9">
                        <div class="content">
                            <div class="btn-controls">
                                <div class="btn-box-row row-fluid">
                                    <a href="groups.aspx" class="btn-box big span6"><i class=" icon-group"></i><b> <%= Methods.getGroupsCount() %> </b>
                                        <p class="text-muted">
                                            Groups</p>
                                    </a><a href="terminals.aspx" class="btn-box big span6"><i class="icon-hdd"></i><b> <%= Methods.getTerminalsCount() %> </b>
                                        <p class="text-muted">
                                            Terminals</p>
                                    </a>
                                </div>
                            </div>
                            <!--/#btn-controls-->
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

</body>
</html>
