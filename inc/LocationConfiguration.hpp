#ifndef LOCATIONCONFIGURATION_HPP
# define LOCATIONCONFIGURATION_HPP

# include "Configuration.hpp"


/// @brief This class is used to store everything in a 'location' block. It inherits from the Configuration class. Everything inside a Configuration object can also exist in a LocationConfiguration object.
class LocationConfiguration : public Configuration
{
	public:
		/* Constructor  */
		LocationConfiguration();
		LocationConfiguration(std::string &s);
		/* Destructor */
		virtual ~LocationConfiguration();

		/* Copy constructor */
		LocationConfiguration(const LocationConfiguration *src);
		LocationConfiguration(const LocationConfiguration &src);


		/* Operation overload = */
		LocationConfiguration& operator = (const LocationConfiguration& src);

		// Methods
		void clean();

		// Setters
		void set_path(std::string &s);
		void set_path(std::string const &s);

		// Getters
		std::string			&get_path();
		const std::string	&get_path() const;
	private:
		std::string		_path;
};

// std::ostream&	operator<<(std::ostream& out, const LocationConfiguration& c)
// {
// 	out << "Path:\n\t" << c.get_path() << std::endl;
// 	out << static_cast<const Configuration&>(c);
// 	return (out);
// }

#endif // LOCATIONCONFIGURATION_HPP