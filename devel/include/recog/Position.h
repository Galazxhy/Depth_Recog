// Generated by gencpp from file recog/Position.msg
// DO NOT EDIT!


#ifndef RECOG_MESSAGE_POSITION_H
#define RECOG_MESSAGE_POSITION_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace recog
{
template <class ContainerAllocator>
struct Position_
{
  typedef Position_<ContainerAllocator> Type;

  Position_()
    {
    }
  Position_(const ContainerAllocator& _alloc)
    {
  (void)_alloc;
    }







  typedef boost::shared_ptr< ::recog::Position_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::recog::Position_<ContainerAllocator> const> ConstPtr;

}; // struct Position_

typedef ::recog::Position_<std::allocator<void> > Position;

typedef boost::shared_ptr< ::recog::Position > PositionPtr;
typedef boost::shared_ptr< ::recog::Position const> PositionConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::recog::Position_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::recog::Position_<ContainerAllocator> >::stream(s, "", v);
return s;
}


} // namespace recog

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsFixedSize< ::recog::Position_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::recog::Position_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::recog::Position_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::recog::Position_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::recog::Position_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::recog::Position_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::recog::Position_<ContainerAllocator> >
{
  static const char* value()
  {
    return "d41d8cd98f00b204e9800998ecf8427e";
  }

  static const char* value(const ::recog::Position_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xd41d8cd98f00b204ULL;
  static const uint64_t static_value2 = 0xe9800998ecf8427eULL;
};

template<class ContainerAllocator>
struct DataType< ::recog::Position_<ContainerAllocator> >
{
  static const char* value()
  {
    return "recog/Position";
  }

  static const char* value(const ::recog::Position_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::recog::Position_<ContainerAllocator> >
{
  static const char* value()
  {
    return "\n"
;
  }

  static const char* value(const ::recog::Position_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::recog::Position_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream&, T)
    {}

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct Position_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::recog::Position_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream&, const std::string&, const ::recog::Position_<ContainerAllocator>&)
  {}
};

} // namespace message_operations
} // namespace ros

#endif // RECOG_MESSAGE_POSITION_H