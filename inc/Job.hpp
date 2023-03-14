#ifndef JOB_HPP
# define JOB_HPP

# include "Server.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include <string>


# include <dirent.h>

# include <sys/stat.h> // stat

class Job
{
	public:
		enum PATH_TYPE
		{
			NOT_FOUND = '0',
			NO_PERMISSIONS = 'X',
			DIRECTORY = 'D',
			FILE_FOUND = 'F',
		};
		/* Constructor  */
			/**
			 * @brief Construct a new Job object
			 *  This object will not be a Client or a server.
			 */
			Job();
			/**
			 * @brief Construct a new Job object
			 * 
			 * @param type This will be the type of the job. Uses the #Defines above. To select the type
			 * @param fd File descriptor of the Client or Server.
			 * @param server A pointer to a server. Will be NULL if its a client.
			 * @param user A pointer to a user. Will be NULL if its a server.
			 */
			Job(int type, int fd, Server *server, Job *client);

		/* Destructor */
			virtual ~Job();

		/* Copy constructor */
			Job(const Job &src);

		/* Operation overload = */
			Job& operator = (const Job& e);

		/* Public Member Functions */
			/// @brief This function is used to check if the job is a server. a Server type is always 'WAIT_FOR_CONNECTION'.
			/// @return this returns true if the job is a server, false if not.
			bool is_server();
			/// @brief This function is used to check if the job is a client job. a Job type is never 'WAIT_FOR_CONNECTION'.
			/// @return this returns true if the job is a client job, false if not.
			bool is_client();

		/* Getters */
			Response &get_response();
			Request &get_request();
			/**
			 * @brief This function returns information about the file. See PATH_TYPE for more information. Used bdekonin minishell code for this. Function that returns information about the file. See PATH_TYPE for more information.
			 * 
			 * @param uri location of the file or directory
			 * @return PATH_TYPE 
			 */
			Job::PATH_TYPE get_path_options(std::string const &uri);
			PATH_TYPE get_path_options(); // Uses this->request->urij

			/**
			 * @brief This clears the request and response of the job.
			 * 
			 */
			void clear();

			/// @brief This function makes sure that when you access the dir 'post' and you have a root of 'www'. It will return 'www/post'.
			/// @param ConfigToChange_path 

			/**
			 * @brief This function makes sure that when you access the dir 'post' and you have a root of 'www'. It will return 'www/post'.
			 * 
			 * @param ConfigToChange_path Uses the config of the job to change the path.
			 */
			void parse_request(std::string &ConfigToChange_path);

			/// @brief This function reads from the file descriptor and saves the header in the request. and the body in the body.
			/// @param fd The file descriptor to read from.

			// /**
			//  * @brief This function reads from the file descriptor and saves the header in the request. and the body in the body.
			//  * 
			//  * @param fd The file descriptor to read from.
			//  */
			// void handle_file(int fd, fd_set *fds);

			/**
			 * @brief This function creates a list of all the files in the directory. 'autoindex' and stores it into the response body.
			 * 
			 * @param config this is the config object. mainly used if there was something wrong in the config. e.g. malloc, read, etc
			 */
			void generate_autoindex_add_respone(Configuration &config);

			/**
			 * @brief This function environment veriables for the CGI. This is alwasy called inside a fork otherwise it will overwrite the parent's environment. so also the shell that it got called from
			 * 
			 */
			void set_environment_variables();
	public:
		size_t			bytes_sent;
		int				type; // Type of connection. See #defines above.
		int				fd; // File descriptor of the connection.
		Server			*server;
		// User			*user; // TODO: change to client class | SAME AS CLIENT
		std::string		address; // only set when its a user; else its empty.
		Request			request;
		Response		response;
		Configuration	correct_config;
		Job				*client;

		/* Set responses functions */
			void set_3xx_response(Configuration &config); // Redirection response
			void set_405_response(Configuration &config); // Method now allowed
			void set_500_response(Configuration &config); // Internal server error Malloc, read, etc error


			void setType(int type);
			void setFd(int fd);
			void setServer(Server *server);
			void setClient(Job *client);
			void setAddress(struct sockaddr_in *address);
			void setAddress(std::string const &address);

			/**
			 * @brief Set the response object
			 * 
			 * @param config Config used for extra information about the configuration
			 * @param code The code of the response. e.g. 200, 404, 500, etc
			 */
			void set_xxx_response(Configuration &config, int code);

			/**
			 * @brief This function will generate a entire autoindex page and store it in the response body.
			 * 
			 * @param job this is the job object.
			 * @param uri this is the uri of the directory.
			 * @param body This is where the autoindex page will be stored.
			 * @return int return > 0 if correct
			 */
			int		generate_autoindex(Job *job, std::string &uri, std::string &body);
	private:
		/* Private Member Functions */
			void	_set_environment_variable(const char *name, const char *value);
			void	_set_environment_variable(const char *name, std::string &value);
			void	_set_environment_variable(const char *name, std::string value);

		public:
			enum JOB_TYPE
			{
				// WAIT_FOR_CONNECTION,
				CLIENT_RESPONSE,
				CLIENT_READ,
				FILE_WRITE,
				FILE_READ,
				CGI_WRITE,
				CGI_READ,

				WAIT_FOR_CONNECTION, // Server Type

				WAIT_FOR_READING, // Client type (Waiting for a Task to be done reading)
				WAIT_FOR_WRITING, // Client type (Waiting for a task to be done writing)
				WAIT_FOR_DELETING, // Client type (Waiting for a task to be done deleting)
				WAIT_FOR_CGIING, // Client type (Waiting for a task to be done CGI)

				READY_TO_READ,
				READY_TO_WRITE,
				READY_TO_DELETE,
				READY_TO_CGI,

				READING, // Task type (Task is reading)
				WRITING, // Task type (Task is writing)
				DELETING, // Task type (Task is deleting)
				CGIING, // Task type (Task is CGI)

				CLIENT_REMOVE, // Client type (Client is REMOVEing)
				TASK_REMOVE, // Task type (Task is REMOVEing)
			};

			const char *job_type_to_char();
			const char *job_type_to_char(JOB_TYPE type);

			bool isClient() const;
			bool isTask() const;
			bool isServer() const;

			Request &_getRequest();
			Response		&_getResponse();

			int fileReader(int fd);
};

// std::ostream&	operator<<(std::ostream& out, const Job &c);
// std::ostream&	operator<<(std::ostream& out, const Job *c);

#endif // JOB_HPP