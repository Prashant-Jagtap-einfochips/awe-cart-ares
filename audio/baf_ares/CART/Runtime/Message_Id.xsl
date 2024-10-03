<?xml version="1.0" encoding="ISO-8859-1" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html" version="1.0" encoding="UTF-8" indent="yes" />
<xsl:template match="/">
<html>
<body>
	<h3>Logger ID </h3>
	<table border="2">
	<tr bgcolor="#9acd32">
		<th colspan="1" rowspan="1" align="center">Logger</th>
		<th colspan="1" rowspan="1" align="center">Token</th>
		<th colspan="1" rowspan="1" align="center">Description</th>
	</tr>
		<xsl:for-each select="root/LOGGER">
			<tr>
			   <td align="left"><xsl:value-of select="@id"/></td>
			   <td align="center"><xsl:value-of select="@Token"/></td>
		  </tr>
		</xsl:for-each>
	</table>
	
	<h3>Error ID </h3>
	<table border="2">
	<tr bgcolor="#9acd32">
		<th colspan="1" rowspan="1" align="center">Error</th>
		<th colspan="1" rowspan="1" align="center">Value</th>
		<th colspan="1" rowspan="1" align="center">Description</th>
	</tr>
		<xsl:for-each select="root/ERROR">
			<tr>
			   <td align="left"><xsl:value-of select="@id"/></td>
			   <td align="center"><xsl:value-of select="@Value"/></td>
		  </tr>
		</xsl:for-each>
	</table>
	
	<h3>Message ID </h3>
	<table border="2">
	<tr bgcolor="#9acd32">
		<th colspan="1" rowspan="2" align="center">Name</th>
		<th colspan="1" rowspan="2" align="center">Token</th>
		<th colspan="3" rowspan="1" align="center">Signals</th>
		<th colspan="1" rowspan="2" align="center">Description</th>
	</tr>
	<tr bgcolor="#9acd32">
		<th colspan="1" rowspan="1" align="center">ID</th>
		<th colspan="1" rowspan="1" align="center">Bit Offset</th>
		<th colspan="1" rowspan="1" align="center">Bit Width</th>
	</tr>
		<xsl:for-each select="root/MESSAGE">
		  <xsl:variable name="rows" select="count(Signal)"/>
			<tr>
						<td rowspan="{$rows+1}">MESSAGE_<xsl:value-of select="@id"/></td>
						<td rowspan="{$rows+1}"><xsl:value-of select="@Token"/></td>
						<td></td>
						<td></td>
						<td></td>
						<td rowspan="{$rows+1}"><xsl:value-of select="Description"/></td>
		  </tr>
			<xsl:for-each select="Signal">
			<tr>
						<td align="center"><xsl:value-of select="@id"/></td>
						<td align="center"><xsl:value-of select="@bit_offset"/></td>
						<td align="center"><xsl:value-of select="@bit_width"/></td>
		  </tr>
			</xsl:for-each>
		</xsl:for-each>
	</table>
</body>
</html>
</xsl:template>
</xsl:stylesheet>
