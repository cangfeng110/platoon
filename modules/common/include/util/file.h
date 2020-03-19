#ifndef MODULES_COMMON_UTIL_FILE_H_
#define MODULES_COMMON_UTIL_FILE_H_

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

#include "google/protobuf/text_format.h"
#include "modules/common/include/util/string_util.h"
//#include "google/protobuf/message_lite.h"
#include "google/protobuf/message.h"
#include "modules/common/include/log.h"

#include "google/protobuf/io/zero_copy_stream_impl.h"
//#include <google/protobuf/io/coded_stream.h>

/**
 * @namespace atd::common::util
 * @brief atd::common::util
 */
namespace atd {
namespace common {
namespace util {

template <typename MessageType>
bool SetProtoToASCIIFile(const MessageType &message, int file_descriptor) {
    //    using google::protobuf::io::ZeroCopyOutputStream;
    //    using google::protobuf::io::FileOutputStream;
    //    using google::protobuf::TextFormat;
    //    if (file_descriptor < 0) {
    //        // << "Invalid file descriptor.";
    //        return false;
    //    }
    //    ZeroCopyOutputStream *output = new FileOutputStream(file_descriptor);
    //    bool success = TextFormat::Print(message, output);
    //    delete output;
    //    close(file_descriptor);
    //    return success;
    return false;
}

/**
 * @brief Sets the content of the file specified by the file_name to be the
 *        ascii representation of the input protobuf.
 * @param message The proto to output to the specified file.
 * @param file_name The name of the target file to set the content.
 * @return If the action is successful.
 */
template <typename MessageType>
bool SetProtoToASCIIFile(const MessageType &message,
                         const std::string &file_name) {
    int fd = open(file_name.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
    if (fd < 0) {
        // << "Unable to open file " << file_name << " to write.";
        return false;
    }
    return SetProtoToASCIIFile(message, fd);
}

/**
 * @brief Parses the content of the file specified by the file_name as ascii
 *        representation of protobufs, and merges the parsed content to the
 *        proto.
 * @param file_name The name of the file to parse whose content.
 * @param message The proto to carry the parsed content in the specified file.
 * @return If the action is successful.
 */
template <typename MessageType>
bool GetProtoFromASCIIFile(const std::string &file_name, MessageType *message) {
//    using google::protobuf::io::ZeroCopyInputStream;
//    using google::protobuf::io::FileInputStream;
//    using google::protobuf::TextFormat;

//    std::ifstream t(file_name);
//    std::string str((std::istreambuf_iterator<char>(t)),
//                    std::istreambuf_iterator<char>());

////        int file_descriptor = open(file_name.c_str(), O_RDONLY);
////        if (file_descriptor < 0) {
////            // << "Failed to open file " << file_name << " in text mode.";
////            // Failed to open;
////            return false;
////        }

//    //due to protobuf-lite donnot has such func, so we don't check the parse success
////    ZeroCopyInputStream *input = new FileInputStream(file_descriptor,-1);
//    bool success = ((google::protobuf::MessageLite*)message)->ParsePartialFromString(str);
////    bool success = TextFormat::ParseFromString(input, (google::protobuf::Message*)message);
//    if (!success) {
//        // << "Failed to parse file " << file_name << " as text proto.";
//    }
////    delete input;
////    close(file_descriptor);
//    return success;

    using google::protobuf::TextFormat;
      using google::protobuf::io::FileInputStream;
      using google::protobuf::io::ZeroCopyInputStream;
      int file_descriptor = open(file_name.c_str(), O_RDONLY);
      if (file_descriptor < 0) {
        AERROR << "Failed to open file " << file_name << " in text mode.";
        // Failed to open;
        return false;
      }

      ZeroCopyInputStream *input = new FileInputStream(file_descriptor);
      bool success = TextFormat::Parse(input, message);
      if (!success) {
        AERROR << "Failed to parse file " << file_name << " as text proto.";
      }
      delete input;
      close(file_descriptor);
      return success;
}



/**
 * @brief Sets the content of the file specified by the file_name to be the
 *        binary representation of the input protobuf.
 * @param message The proto to output to the specified file.
 * @param file_name The name of the target file to set the content.
 * @return If the action is successful.
 */
template <typename MessageType>
bool SetProtoToBinaryFile(const MessageType &message,
                          const std::string &file_name) {
    std::fstream output(file_name,
                        std::ios::out | std::ios::trunc | std::ios::binary);
    return message.SerializeToOstream(&output);
}

/**
 * @brief Parses the content of the file specified by the file_name as binary
 *        representation of protobufs, and merges the parsed content to the
 *        proto.
 * @param file_name The name of the file to parse whose content.
 * @param message The proto to carry the parsed content in the specified file.
 * @return If the action is successful.
 */
template <typename MessageType>
bool GetProtoFromBinaryFile(const std::string &file_name,
                            MessageType *message) {
    std::fstream input(file_name, std::ios::in | std::ios::binary);
    if (!input.good()) {
        // << "Failed to open file " << file_name << " in binary mode.";
        return false;
    }
    //due to protobuf-lite donnot has this func
    //  if (!((google::protobuf::Message*)message)->ParseFromIstream(&input)) {
    //    // << "Failed to parse file " << file_name << " as binary proto.";
    //    return false;
    //  }
    return true;
}

/**
 * @brief Parses the content of the file specified by the file_name as a
 *        representation of protobufs, and merges the parsed content to the
 *        proto.
 * @param file_name The name of the file to parse whose content.
 * @param message The proto to carry the parsed content in the specified file.
 * @return If the action is successful.
 */
template <typename MessageType>
bool GetProtoFromFile(const std::string &file_name, MessageType *message) {
        // Try the binary parser first if it's much likely a binary proto.
        if (EndWith(file_name, ".bin")) {
            return GetProtoFromBinaryFile(file_name, message) ||
                    GetProtoFromASCIIFile(file_name, message);
        }

        return GetProtoFromASCIIFile(file_name, message) ||
                GetProtoFromBinaryFile(file_name, message);
    return GetProtoFromASCIIFile(file_name, message);
}

/**
 * @brief Get file content as string.
 * @param file_name The name of the file to read content.
 * @param content The file content.
 * @return If the action is successful.
 */
bool GetContent(const std::string &file_name, std::string *content);


/**
 * @brief Get absolute path by concatenating prefix and relative_path.
 * @return The absolute path.
 */
std::string GetAbsolutePath(const std::string& prefix,
                            const std::string& relative_path);

/**
 * @brief Check if the path exists.
 * @return If the path exists.
 */
bool PathExists(const std::string &path);

/**
 * @brief Check if the directory specified by directory_path exists
 *        and is indeed a directory.
 * @param directory_path Directory path.
 * @return If the directory specified by directory_path exists
 *         and is indeed a directory.
 */
bool DirectoryExists(const std::string &directory_path);

/**
 * @brief Copy a file.
 * @param from The file path to copy from.
 * @param to The file path to copy to.
 * @return If the action is successful.
 */
bool CopyFile(const std::string &from, const std::string &to);

/**
 * @brief Copy a directory.
 * @param from The path to copy from.
 * @param to The path to copy to.
 * @return If the action is successful.
 */
bool CopyDir(const std::string &from, const std::string &to);

/**
 * @brief Copy a file or directory.
 * @param from The path to copy from.
 * @param to The path to copy to.
 * @return If the action is successful.
 */
bool Copy(const std::string &from, const std::string &to);

/**
 * @brief Check if a specified directory specified by directory_path exists.
 *        If not, recursively create the directory (and its parents).
 * @param directory_path Directory path.
 * @return If the directory does exist or its creation is successful.
 */
bool EnsureDirectory(const std::string &directory_path);

/**
 * @brief Remove all the files under a specified directory. Note that
 *        sub-directories are NOT affected.
 * @param directory_path Directory path.
 * @return If the action is successful.
 */
bool RemoveAllFiles(const std::string &directory_path);

/**
 * @brief List sub-directories.
 * @param directory_path Directory path.
 * @return A vector of sub-directories, without the directory_path prefix.
 */
std::vector<std::string> ListSubDirectories(const std::string &directory_path);

}  // namespace util
}  // namespace common
}  // namespace atd

#endif  // MODULES_COMMON_UTIL_FILE_H_
