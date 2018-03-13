#ifndef XXX_RESOURCE_HPP
#define XXX_RESOURCE_HPP

#include <string>

namespace xxx {
   class RenderManager;
   class Logger;

   // Template for data in function. Automatically has get and set built in
   template <typename T> class data {
   public:
      data() {};
      data(T newValue) : value(newValue) {};
      inline void set(T newValue) { value = newValue; };
      inline T get(void) { return value; };

   private:
      T value;
   };


   struct Asset {
      Asset(std::string fileLocIn, std::string assetNameIn = NULL, uint guidIn = 0)
          : fileLocation(fileLocIn), assetName(assetNameIn), guid(guidIn) {}

      std::string fileLocation, assetName;
      uint guid;
   };




   class Resource {
   /*Note-----------------------------------------------------------------------
   - Wanted to load files in constructors but we don't know the size before file is loaded
   ---------------------------------------------------------------------------*/
   public:
      virtual ~Resource();

      virtual unsigned int getResource() = 0;
      //getNumParents
      //getChildren

      //addchild
      //add parent?

      static Logger* logger;


      virtual void loadToGraphics(RenderManager* renderMan);

   protected:
      Resource();



      //numParents
      //vector of children pointers
      //GUID
   };


   class Model {
      Model();
      ~Model();
   };


   //All of these have been assigned public, need to find out if this is correct
   class Mesh : public Resource {
   public:
      Mesh(float* vertsIn, float* normsIn,
           float* uvsIn, uint* facesIn,
            uint nVertsIn, uint nFaceIn);
      ~Mesh();

      inline uint getResource() { return VAO; };
      void loadToGraphics(RenderManager* renderMan);

      void getMesh(float** vertsOut, float** normsOut,
                   float** uvsOut, uint** facesOut,
                   uint &nVertsOut, uint &nFacesOut);

      // This is a duplicate of getResource
      inline uint getVAO() { return VAO; };
      inline void setVAO(uint vaoIn) { VAO = vaoIn; };

   private:
      float *verts, *norms, *uvs;
      uint *faces, nVerts, nFaces, VAO;
   };

   class Skeleton : public Resource {
   public:
      Skeleton();
      ~Skeleton();

      unsigned int getResource() { return 0; };

   private:
      //resources
      //graphics ids
   };


   class Animation : public Resource {
   public:
      Animation();
      ~Animation();

      unsigned int getResource() { return 0; };

   private:
      //resources
      //graphics ids
   };

   class Material : public Resource {
   public:
      Material();
      ~Material();

      unsigned int getResource() { return 0; };

   private:
      //resources
      //graphics ids
   };




   class Font : public Resource {
   public:
      Font();
      ~Font();

      unsigned int getResource() { return 0; };

   private:
      //resources
      //graphics ids
   };

}





#endif // XXX_RESOURCE_HPP
