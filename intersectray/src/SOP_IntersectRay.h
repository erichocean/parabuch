#ifndef __SOP_IntersectRay_h__
#define __SOP_IntersectRay_h__

#include <SOP/SOP_Node.h>
namespace INTERSECT_RAY {

class Timer {
private:
    double begTime;
public:
    void start()
    {
        begTime = clock();
    }

    double current()
    {
        return (difftime(clock(), begTime) / CLOCKS_PER_SEC);
    }
    bool isTimeout(double seconds) 
    {
        return seconds >= current();
    }
};

class SOP_IntersectRay : public SOP_Node
{
public:
	     SOP_IntersectRay(OP_Network *net, const char *name, OP_Operator *op);
    virtual ~SOP_IntersectRay();

    static PRM_Template		 myTemplateList[];
    static OP_Node		*myConstructor(OP_Network*, const char *,
							    OP_Operator *);

    /// This method is created so that it can be called by handles.  It only
    /// cooks the input group of this SOP.  The geometry in this group is
    /// the only geometry manipulated by this SOP.
    virtual OP_ERROR		 cookInputGroups(OP_Context &context, 
						int alone = 0);

protected:
    /// Method to cook geometry for the SOP
    virtual OP_ERROR		 cookMySop(OP_Context &context);

private:
    void	getGroups(UT_String &str)
            { 
                evalString(str, "group", 0, 0); 
            }
    float	EDGELENGTH(float t)		{ return evalFloat("edgelength", 0, t); }
    float	PRIMAREA(float t)		{ return evalFloat("primarea", 0, t); }
    float	VERBOSE(float t)		{ return evalInt("verbose", 0, t); }
    float	GENERATEPOINTS(float t)		{ return evalInt("generatepoints", 0, t); }

    GU_DetailGroupPair	 myDetailGroupPair;
    const   GB_PrimitiveGroup	*myGroup;
};
} // End INTERSECT_RAY namespace

#endif
