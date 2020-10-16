#pragma once

//-----------------------------------------------------------------------------
// mvApp
//
//     - This class acts as the primary manager for a DearPyGui app,
//       with the following responsibilities:
//
//         * Adding/Removing/Modifying AppItems
//         * Callback routing
//         * AppItem parent deduction
//     
//     - This class can eventually just contain all static members & methods
//     
//-----------------------------------------------------------------------------

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <vector>
#include <map>
#include <stack>
#include <string>
#include <atomic>
#include <queue>
#include <chrono>
#include <mutex>
#include <thread>
#include "mvAppItem.h"
#include "mvPythonParser.h"
#include "mvEventHandler.h"

//-----------------------------------------------------------------------------
// Typedefs for chrono's ridiculously long names
//-----------------------------------------------------------------------------
typedef std::chrono::high_resolution_clock clock_;
typedef std::chrono::duration<double, std::ratio<1> > second_;
typedef std::map<ImGuiStyleVar, ImVec2> mvStyle;
#if defined (_WIN32)
typedef std::chrono::steady_clock::time_point time_point_;
#elif defined(__APPLE__)
typedef std::chrono::steady_clock::time_point time_point_;
#else
typedef std::chrono::system_clock::time_point time_point_;
#endif

namespace Marvel {

    //-----------------------------------------------------------------------------
    // Forward Declarations
    //-----------------------------------------------------------------------------
    class mvWindowAppitem;
    class mvThreadPool;
    class mvTextEditor;
    class mvWindow;
    struct mvColor;
    
    //-----------------------------------------------------------------------------
    // mvApp
    //-----------------------------------------------------------------------------
    class mvApp : public mvEventHandler
    {

        friend class mvWindow;
        friend class mvWindowsWindow;
        friend class mvLinuxWindow;
        friend class mvAppleWindow;

        struct OrderedItem
        {
            mvAppItem* item;   // new item to add
            std::string prev; // what item to add item after
        };

        struct NewRuntimeItem
        {
            mvAppItem*  item;   // new item to add
            std::string before; // what item to add new item before
            std::string parent; // what parent to add item to (if not using before)
        };

        struct AsyncronousCallback
        {
            PyObject* name;       // name of function to run
            PyObject* data;       // any data need by the function
            PyObject* returnname; // optional return function
        };

        struct StolenChild
        {
            std::string item;   // item to steal
            std::string parent; // what item to add stolen item before
            std::string before; // what parent to add item to (if not using before)
        };

    public:

        mvApp          (const mvApp& other) = delete;
        mvApp          (mvApp&& other)      = delete;
        mvApp operator=(const mvApp& other) = delete;
        mvApp operator=(mvApp&& other)      = delete;

        static mvApp*            GetApp              ();
        static void              DeleteApp           ();
        static const char*       GetVersion          () { return MV_SANDBOX_VERSION; }
        static bool              IsAppStarted        () { return s_started; }
        static void              SetAppStarted       ();
        static void              SetAppStopped       ();

        ~mvApp();

        //-----------------------------------------------------------------------------
        // Rendering
        //-----------------------------------------------------------------------------
        void                     precheck        (); // precheck before the main render loop has started
        void                     firstRenderFrame(); // only ran during first frame
        bool                     prerender       (); // pre rendering (every frame)
        void                     render          (); // actual render loop
        void                     postrender      (); // post rendering (every frame)
        
        //-----------------------------------------------------------------------------
        // App Settings
        //-----------------------------------------------------------------------------
        void                     setVSync          (bool value) { m_vsync = value; }
        void                     setResizable      (bool value) { m_resizable = value; }
        void                     setWindowSize     (unsigned width, unsigned height);
        void                     setActualSize     (unsigned width, unsigned height);			
        void                     setMainPos        (int x, int y);			
        void                     setActiveWindow   (const std::string& window) { m_activeWindow = window; }
        void                     setGlobalFontScale(float scale);
        void                     setViewport       (mvWindow* viewport) { m_viewport = viewport; }
        void                     setTitle          (const std::string& title) { m_title = title; }
        void                     setFont(const std::string& file, float size = 13.0f, const std::string& glyphRange = "",
                                    std::vector<std::array<ImWchar, 3>> customRanges = {},
                                    std::vector<ImWchar> chars= {});
        
        const std::string&       getActiveWindow   () const { return m_activeWindow; }
        std::vector<mvAppItem*>& getWindows        ()       { return m_windows; }
        float&                   getGlobalFontScale()       { return m_globalFontScale; }
        int                      getActualWidth    () const { return m_actualWidth; }
        int                      getActualHeight   () const { return m_actualHeight; }
        ImGuiStyle&              getStyle          ()       { return m_newstyle; }
        mvWindow*                getViewport       ()       { return m_viewport; }
        bool                     getVSync          () const { return m_vsync; }
        bool                     getResizable      () const { return m_resizable; }
        
        //-----------------------------------------------------------------------------
        // Styles/Themes
        //-----------------------------------------------------------------------------
        void                     setAppTheme      (const std::string& theme);
        void                     setThemeItem     (long item, mvColor color);
        void                     setStyleChanged  () { m_styleChange = true; }
                                 
        const std::string&       getAppTheme () const { return m_theme; }
        mvColor                  getThemeItem(long item);

        //-----------------------------------------------------------------------------
        // Concurrency
        //-----------------------------------------------------------------------------
        void                     setThreadPoolTimeout          (double time)   { m_threadPoolTimeout = time; }
        void                     setThreadCount                (unsigned count){ m_threads = count; }
        void                     activateThreadPool            ()              { m_threadPool = true; }
        void                     setThreadPoolHighPerformance  ()              { m_threadPoolHighPerformance = true; }
                                 
        bool                     checkIfMainThread             () const;
        double                   getThreadPoolTimeout          () const { return m_threadPoolTimeout; }
        unsigned                 getThreadCount                () const { return m_threads; }
        bool                     usingThreadPool               () const { return m_threadPool; }
        bool                     usingThreadPoolHighPerformance() const { return m_threadPoolHighPerformance; }

        //-----------------------------------------------------------------------------
        // AppItem Operations
        //-----------------------------------------------------------------------------
        bool                     addItem           (mvAppItem* item);
        bool                     addItemAfter      (const std::string& prev, mvAppItem* item);
        bool                     addWindow         (mvAppItem* item);
        bool                     addRuntimeItem    (const std::string& parent, const std::string& before, mvAppItem* item);
        bool                     moveItem          (const std::string& name, const std::string& parent, const std::string& before);
        void                     deleteItem        (const std::string& name) { if(name!="MainWindow") m_deleteQueue.push(name); }
        void                     deleteItemChildren(const std::string& name) { m_deleteChildrenQueue.push(name); }
        void                     moveItemUp        (const std::string& name) { m_upQueue.push(name); }
        void                     moveItemDown      (const std::string& name) { m_downQueue.push(name); }
        mvAppItem*               getItem           (const std::string& name, bool ignoreRuntime = false);
        mvAppItem*               getRuntimeItem    (const std::string& name);
        mvWindowAppitem*         getWindow         (const std::string& name);
        
        //-----------------------------------------------------------------------------
        // Parent stack operations
        //     - used for automatic parent deduction
        //-----------------------------------------------------------------------------
        void                     pushParent(mvAppItem* item); // pushes parent onto stack
        mvAppItem*               popParent();                 // pop parent off stack and return it
        mvAppItem*               topParent();                 // returns top parent without popping

        //-----------------------------------------------------------------------------
        // Callbacks
        //-----------------------------------------------------------------------------
        void                     runReturnCallback(PyObject* callback, const std::string& sender, PyObject* data);
        void                     runCallback      (PyObject* callback, const std::string& sender, PyObject* data = nullptr);
        void                     runAsyncCallback (PyObject* callback, PyObject* data, PyObject* returnname);
        void                     addMTCallback    (PyObject* name, PyObject* data, PyObject* returnname = nullptr);

        //-----------------------------------------------------------------------------
        // Timing
        //-----------------------------------------------------------------------------
        float                    getDeltaTime() const { return m_deltaTime; }
        double                   getTotalTime() const { return m_time; }

        std::map<std::string, mvPythonParser>* getParsers() { return m_parsers; }
            
    private:

        //-----------------------------------------------------------------------------
        // Post Rendering Methods
        //     - actually performs queued operations
        //-----------------------------------------------------------------------------
        void postDeleteItems();
        void postAddItems   ();
        void postAddPopups  ();
        void postMoveItems  ();
        void postAsync      ();

        mvApp();

        void routeInputCallbacks();
        void updateStyle();
        
    private:

        static mvApp* s_instance;
        static bool   s_started;

        mvWindow*                              m_viewport = nullptr;
        std::string                            m_activeWindow = "MainWindow";
        std::stack<mvAppItem*>                 m_parents;
        std::vector<mvAppItem*>                m_windows;
        int                                    m_actualWidth = 1280;
        int                                    m_actualHeight = 800;
        int                                    m_mainXPos = 100;
        int                                    m_mainYPos = 100;
        std::string                            m_title = "DearPyGui";
        std::map<std::string, mvPythonParser>* m_parsers;
        
        // appearance
        std::string m_theme = "Dark";
        float       m_globalFontScale = 1.0f;
        ImGuiStyle  m_newstyle;
        bool        m_firstRender = true;
        bool        m_styleChange = true;
        bool        m_vsync = true;
        bool        m_resizable = true;

        // fonts
        std::string                         m_fontFile;
        std::string                         m_fontGlyphRange;
        float                               m_fontSize = 13.0f;
        std::vector<std::array<ImWchar, 3>> m_fontGlyphRangeCustom;
        std::vector<ImWchar>                m_fontGlyphChars;

        // runtime widget modifications
        std::queue<std::string>     m_deleteChildrenQueue;
        std::queue<std::string>     m_deleteQueue;
        std::queue<std::string>     m_upQueue;
        std::queue<std::string>     m_downQueue;
        std::vector<NewRuntimeItem> m_newItemVec;
        std::vector<OrderedItem>    m_orderedVec;
        std::queue<StolenChild>     m_moveVec;

        // timing
        float  m_deltaTime; // time since last frame
        double m_time;      // total time since starting
        
        // concurrency
        std::queue<AsyncronousCallback>  m_asyncReturns;
        std::vector<AsyncronousCallback> m_asyncCallbacks;
        mvThreadPool*                    m_tpool = nullptr;
        mutable std::mutex               m_mutex;
        std::thread::id                  m_mainThreadID;
        bool                             m_threadPool = false;                // is threadpool activated
        double                           m_threadPoolTimeout = 30.0;          // how long til trying to delete pool
        unsigned                         m_threads = 2;                       // how many threads to use
        bool                             m_threadPoolHighPerformance = false; // when true, use max number of threads
        double                           m_threadTime = 0.0;                  // how long threadpool has been active
        time_point_                      m_poolStart;                         // threadpool start time

    };

}