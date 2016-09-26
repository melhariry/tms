<%@ Page Language="C#" AutoEventWireup="true" CodeFile="Error.aspx.cs" Inherits="Error" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
    <head runat="server">
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <link rel="shortcut icon" type="image/x-icon" href="Resources/img/favicon.ico" />
        <link type="text/css" href="Resources/css/bootstrap.min.css" rel="stylesheet" />
        <link type="text/css" href="Resources/css/theme.css" rel="stylesheet" />
        <link type="text/css" href="Resources/css/font-awesome.css" rel="stylesheet" />
        <link type="text/css" href='http://fonts.googleapis.com/css?family=Open+Sans:400italic,600italic,400,600' rel='stylesheet' />
        <title>MTMS - Error</title>
    </head>
    <body>

        <div class="navbar navbar-fixed-top">
            <div class="navbar-inner">
                <div class="container">
                    <a class="btn btn-navbar" data-toggle="collapse" data-target=".navbar-inverse-collapse">
                        <i class="icon-reorder shaded"></i>
                    </a>

                    <a class="brand" href="index.aspx">MTMS
                    </a>
                </div>
            </div>
            <!-- /navbar-inner -->
        </div>
        <!-- /navbar -->

        <div class="wrapper">
            <div class="container">
                <div class="row">
                    <div class="module module-login span4 offset4">
                        <form id="login_form" runat="server" class="form-vertical">
                            <div class="module-head">
                                <h3>An error occured</h3>
                            </div>
                            <div class="module-body">
                                <%
                                    int code = Convert.ToInt32(Context.Request.Params["code"]);
                                    switch (code)
                                    {
                                        case 404:
                                            Response.Write("Error 404 - Page not found, please check the url.");
                                            break;
                                        case 500:
                                            Response.Write("Error 500 - Internal server error, please contact the server adminstrator");
                                            break;
                                        case 503:
                                            Response.Write("Error 503 - Service unavailable");
                                            break;
                                        default:
                                            Response.Write("An error has occured, please contact tech support");
                                            break;
                                    } %>
                            </div>
                        </form>
                    </div>
                </div>
            </div>
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
