template <typename... Args>
ShaderProgram::ShaderProgram(const ShaderObject& s1, const ShaderObject& s2, const Args&... rest)
{
	shader_program_ = glCreateProgram();
	attach_recursive(s1, s2, rest...);

	if (!link())
	{
		std::cerr << "Shader program linking failed. Info log:" << std::endl << get_info_log();
		throw std::runtime_error("Shader program linking failed.");
	}
}

template <typename... Args>
void ShaderProgram::attach_recursive(const ShaderObject& s, const Args&... rest)
{
	glAttachShader(shader_program_, s);
	attach_recursive(rest...);
}
