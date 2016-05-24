<%@ Page Language="C#" AutoEventWireup="true" CodeFile="Login.aspx.cs" Inherits="login" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
	<meta name="viewport" content="width=device-width, initial-scale=1.0"/>
	<link type="text/css" href="Resources/css/bootstrap.min.css" rel="stylesheet"/>
	<link type="text/css" href="Resources/css/bootstrap-responsive.min.css" rel="stylesheet"/>
	<link type="text/css" href="Resources/css/theme.css" rel="stylesheet"/>
	<link type="text/css" href="Resources/css/font-awesome.css" rel="stylesheet"/>
	<link type="text/css" href='http://fonts.googleapis.com/css?family=Open+Sans:400italic,600italic,400,600' rel='stylesheet'/>
    <title>MTMS - Login</title>
</head>
<body>

    <div class="navbar navbar-fixed-top">
		<div class="navbar-inner">
			<div class="container">
				<a class="btn btn-navbar" data-toggle="collapse" data-target=".navbar-inverse-collapse">
					<i class="icon-reorder shaded"></i>
				</a>

			  	<a class="brand" href="index.aspx">
			  		MTMS
			  	</a>
			</div>
		</div><!-- /navbar-inner -->
	</div><!-- /navbar -->
    
	<div class="wrapper">
		<div class="container">
			<div class="row">
				<div class="module module-login span4 offset4">
					<form id="login_form" runat="server" class="form-vertical">
						<div class="module-head">
							<h3>Sign In</h3>
						</div>
						<div class="module-body">
                            <% PrintStatus(); %>
							<div class="control-group">
								<div class="controls row-fluid">
									<asp:TextBox runat="server" CssClass="span12" id="inputEmail" placeholder="Username"/>
                                    <asp:RequiredFieldValidator runat="server" ErrorMessage="* Username is required" ControlToValidate="inputEmail" ForeColor="Red" />
								</div>
							</div>
							<div class="control-group">
								<div class="controls row-fluid">
									<asp:TextBox runat="server" CssClass="span12" TextMode="Password"  id="inputPassword" placeholder="Password"/>
                                    <asp:RequiredFieldValidator runat="server" ErrorMessage="* Password is required" ControlToValidate="inputPassword" ForeColor="Red"/>
								</div>
							</div>
						</div>
						<div class="module-foot">
							<div class="control-group">
								<div class="controls clearfix">
									<asp:Button ID="Login" runat="server" CssClass="btn btn-primary pull-right" Text="Login" OnClick="Login_Click"></asp:Button>
								</div>
							</div>
						</div>
					</form>
				</div>
			</div>
		</div>
	</div><!--/.wrapper-->

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
