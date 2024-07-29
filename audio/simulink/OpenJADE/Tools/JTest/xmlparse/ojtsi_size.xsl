<?xml version="1.0" encoding="ISO-8859-1" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html" version="1.0" encoding="UTF-8" indent="yes" />
<xsl:template match="/">
<html>
<body>
	<h3>Memory Breakdown </h3>
	<table border="2">
	<tr bgcolor="#9acd32">
		<th colspan="1" rowspan="1" align="center">MEMORY_BLOCK</th>
		<th colspan="1" rowspan="1" align="center">MEMORY_SECTION</th>
		<th colspan="1" rowspan="1" align="center">MEMORY_SECTION_LENGTH</th>
		<th colspan="1" rowspan="1" align="center">MEMORY</th>
		<th colspan="1" rowspan="1" align="center">MEMORY_TOTAL</th>
		<th colspan="1" rowspan="1" align="center">CONTRIBUTOR</th>
		<th colspan="1" rowspan="1" align="center">SIZE</th>
	</tr>
	<xsl:for-each select="root/MEMORY_BLOCK">
		<xsl:variable name="memory_block" select="@id"/>
		<xsl:for-each select="MEMORY_SECTION">
			<xsl:variable name="memory_section" select="@id"/>
			<xsl:variable name="memory_section_length" select="@length"/>
			<xsl:variable name="memory_count" select="count(MEMORY)"/>
			<xsl:if test="$memory_count = '0'">
					<tr>
						<td align="left"><xsl:value-of select="$memory_block"/></td>
						<td align="left"><xsl:value-of select="$memory_section"/></td>
						<td align="left"><xsl:value-of select="$memory_section_length"/></td>
						<td align="left"></td>
						<td align="left"></td>
						<td align="left"></td>
						<td align="left"></td>
					</tr>
		  </xsl:if>
			<xsl:for-each select="MEMORY">
				<xsl:variable name="memory" select="@id"/>
				<xsl:variable name="memory_total" select="@total"/>
				<xsl:variable name="contributor_count" select="count(CONTRIBUTOR)"/>
				<xsl:if test="$contributor_count = '0'">
					<tr>
						<td align="left"><xsl:value-of select="$memory_block"/></td>
						<td align="left"><xsl:value-of select="$memory_section"/></td>
						<td align="left"><xsl:value-of select="$memory_section_length"/></td>
						<td align="left"><xsl:value-of select="$memory"/></td>
						<td align="left"><xsl:value-of select="$memory_total"/></td>
						<td align="left"></td>
						<td align="left"></td>
					</tr>
		    </xsl:if>
				<xsl:for-each select="CONTRIBUTOR">
					<tr>
						<td align="left"><xsl:value-of select="$memory_block"/></td>
						<td align="left"><xsl:value-of select="$memory_section"/></td>
						<td align="left"><xsl:value-of select="$memory_section_length"/></td>
						<td align="left"><xsl:value-of select="$memory"/></td>
						<td align="left"><xsl:value-of select="$memory_total"/></td>
						<td align="left"><xsl:value-of select="@id"/></td>
						<td align="left"><xsl:value-of select="@size"/></td>
					</tr>
				</xsl:for-each>
			</xsl:for-each>
		</xsl:for-each>
	</xsl:for-each>
	</table>
</body>
</html>
</xsl:template>
</xsl:stylesheet>
