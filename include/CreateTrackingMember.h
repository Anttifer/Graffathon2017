#ifndef CREATETRACKINGMEMBER_H
#define CREATETRACKINGMEMBER_H

#define CREATE_TRACKING_MEMBER(type, name)\
public:\
	const type& name           (void)          const { return *name##_; }\
	void        set_##name     (const type& i)\
	{\
		if (name##_ == &name##_internal_)\
			*name##_ = i;\
	}\
	void        name##_track   (type& t)\
	{\
		name##_internal_ = name##_default_;\
		name##_default_  = t;\
		name##_          = &t;\
	}\
	void        name##_untrack (void)\
	{\
		name##_default_ = name##_internal_;\
		name##_         = &name##_internal_;\
	}\
private:\
	type  name##_default_;\
	type  name##_internal_ = name##_default_;\
	type* name##_          = &name##_internal_;

#endif // CREATETRACKINGMEMBER_H
