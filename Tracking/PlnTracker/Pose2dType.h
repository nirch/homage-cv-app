
#ifndef _pose2f_type_H_
#define _pose2f_type_H_





typedef struct pose2f_type {

	union {
		struct {
			float x;
			float y;
		};
		vec2f_type p;
	};

	float scale;
	float angle;

	float quality;


} pose2f_type;



typedef struct pose2d_type {

	union {
		struct {
			double x;
			double y;
		};
		vec2d_type p;
	};

	double scale;
	double angle;

	float qulity;


} pose2d_type;




#endif


