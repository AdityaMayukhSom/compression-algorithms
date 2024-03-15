#include <string>
class Profiler {
   public:
    virtual float calculate_space_saving() const = 0;
    virtual float calculate_compression_ratio(const std::string input_file_path,
                                              const std::string output_file_path) const = 0;
};