#ifndef M_UTILITIES_H
#define M_UTILITIES_H

inline std::tuple<int,int,int> hexToRGB(const std::string& hex) {
    std::stringstream ss;
	std::string hex_str = hex.substr(hex[0] == '#' ? 1 : 0);
	if (hex_str.length() != 6) throw std::invalid_argument("bad argument, incorrect format for hex");
	
	std::array<int,3> rgb;
	for (int i = 0; i <= 2; i++)
	{
		ss.clear();
		std::cout << hex_str.substr(i*2,2) << '\n';
		ss << hex_str.substr(i*2,2);
		ss >> std::hex >> rgb[i];
		std::cout << ss.str() << '\n' << "-===============-\n";
	};
	for (auto i : rgb) std::cout << std::dec << i; 
	return {rgb[0], rgb[1], rgb[2]};
}

#endif
