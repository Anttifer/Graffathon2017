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

template <typename T>
UBO<T>::UBO(GLuint index) :
	index_(index)
{
	glBindBuffer(GL_UNIFORM_BUFFER, buffer_);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(T), nullptr, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferBase(GL_UNIFORM_BUFFER, index_, buffer_);
}

template <typename T>
UBO<T>::~UBO(void)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, index_, 0);
}

template <typename T>
void UBO<T>::update_buffer(void)
{
	glBindBuffer(GL_UNIFORM_BUFFER, buffer_);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), static_cast<T*>(this));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
