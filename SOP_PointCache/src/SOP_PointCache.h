#ifndef __SOP_PointCache_h__
#define __SOP_PointCache_h__

#include <SOP/SOP_Node.h>

namespace PC2SOP {

/// PC2 format header:
/// http://www.footools.com/plugins/docs/PointCache2_README.html#pc2format
struct PC2Header
{
	char magic[12];
	int version;
	int numPoints;
	float startFrame;
	float sampleRate;
	int numSamples;
};

/// Interpolation types
enum PC2_Interpol_Type
{
    PC2_NONE,
    PC2_LINEAR,
    PC2_CUBIC,
};

/// PC2 handler class
class PC2_File
{
public: 
    PC2_File();
    PC2_File(UT_String *file, int load = 0)
    {
        filename = new UT_String(file->buffer());
        filename->harden();        
        if (file && load)
        {
            int success = 0;
            success = loadFile(file); 
            loaded = success; 
         }
    }
    ~PC2_File();
    int loadFile(UT_String *file);
    int loadFile() { return loadFile(filename);}
    int isLoaded() { return loaded;}
    int setUnload() {loaded = 0; return 1;} 
    int getPArray(int sample, int steps, float *pr);
    UT_String * getFilename() {return filename;}
    PC2Header *header;
    
private:
    const char *buffer[32];
    int loaded;
    UT_String  *filename;
};

/// Main class it was modeled after SOP_PointWave example from HDK.
class SOP_PointCache : public SOP_Node
{
public:
    // Usual stuff:
	SOP_PointCache(OP_Network *net, const char *name, OP_Operator *op);
    virtual ~SOP_PointCache();
    static  PRM_Template myTemplateList[];
    static  OP_Node		 *myConstructor(OP_Network*, const char *, OP_Operator *);
    virtual OP_ERROR     cookInputGroups(OP_Context &context, int alone = 0);

    // This triggers reallocation of points[] array only for cases filename has changed.
    static  int          triggerReallocate(void *data, int, fpreal, const PRM_Template *)
    {  
        SOP_PointCache *node = (SOP_PointCache*) data;
        node->reallocate = true;
        return 1;
    };

protected:
    // Method to cook geometry for the SOP
    virtual OP_ERROR		 cookMySop(OP_Context &context);

private:
    // Parameters evaluation:
    void	getGroups(UT_String &str){ evalString(str, "group", 0, 0); }
    void    PGROUP(UT_String &str)   { evalString(str, "pGroup", 0, 0); }
    void    FILENAME(UT_String &str, float t){ return evalString(str, "filename", 0, t);}
    int     COMPUTENORMALS(float t)  { return evalInt("computeNormals", 0, t);}
    int     FLIP(float t)            { return evalInt("flip", 0, t);}
    int     ADDREST(float t)         { return evalInt("addrest", 0, t);}
    int     RELATIVE(float t)        { return evalInt("relative", 0, t);}
    void    SETINTERPOL(UT_String &v, float t) 
        { 
            setString(v, CH_STRING_LITERAL, "interpol", 0, t);
        }
    
    /// I don't know how to make int based list...
    int INTERPOL(UT_String &str, float t)
        {   
            evalString(str, "interpol", 0, t);
            return SYSatoi(str.buffer());
        }

    /// This variable is used together with the call to the "checkInputChanged"
    /// routine to notify the handles (if any) if the input has changed.
    GU_DetailGroupPair	 myDetailGroupPair;

    /// This is the group of geometry to be manipulated by this SOP and cooked
    /// by the method "cookInputGroups".
    const GA_PointGroup	*myGroup;
    float               *points;
    float               *points_zero;
    int                 dointerpolate;
    bool                reallocate;
    PC2_File            *pc2;
};
} // End PC2SOP namespace

#endif
