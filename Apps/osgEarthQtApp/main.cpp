/*  -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2010 Robert Osfield

    This application is open source and may be redistributed and/or modified
    freely and without restriction, both in commercial and non commercial applications,
    as long as this copyright notice is maintained.

    This application is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#include <osgQOpenGL/osgQOpenGLWidget>

#include <osg/CoordinateSystemNode>
#include <osgDB/ReadFile>
#include <osgUtil/Optimizer>

#include <osg/Switch>
#include <osg/Types>
#include <osgText/Text>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/AnimationPathManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/SphericalManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/TerrainManipulator>
#include <osgGA/TrackballManipulator>

#include <osgGA/Device>

#include <osgEarth/MapNode>
#include <osgEarth/SpatialReference>

#include <osgEarth/EarthManipulator>
#include <osgEarth/Ephemeris>
#include <osgEarth/GeoCommon>
#include <osgEarth/GeoTransForm>
#include <osgEarth/Sky>
#include <osgEarth/SpatialReference>
#include <osgEarth/MapNode>
#include <osgEarth/EarthManipulator>
#include <osgEarth/ExampleResources>
#include <osgEarth/MapNode>
#include <osgEarth/Threading>
#include <osgEarth/ShaderGenerator>

#include <QApplication>
#include <QSurfaceFormat>

#include <iostream>


int main(int argc, char** argv)
{

    osgEarth::initialize();

    QSurfaceFormat format = QSurfaceFormat::defaultFormat();

#ifdef OSG_GL3_AVAILABLE
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setOption(QSurfaceFormat::DebugContext);
#else
    format.setVersion(2, 0);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setOption(QSurfaceFormat::DebugContext);
#endif
    format.setDepthBufferSize(24);
    // format.setAlphaBufferSize(8);
    format.setSamples(8);
    format.setStencilBufferSize(8);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication app(argc, argv);

    // use an ArgumentParser object to manage the program arguments.
    osg::ArgumentParser arguments(&argc, argv);


    osgQOpenGLWidget widget(&arguments);

    QObject::connect(&widget, &osgQOpenGLWidget::initialized, [&arguments, &widget] {
        

       // This is normally called by Viewer::run but we are running our frame loop manually so we
        // need to call it here.
        widget.getOsgViewer()->setReleaseContextAtEndOfFrameHint(false);

        // Tell the database pager to not modify the unref settings
        widget.getOsgViewer()->getDatabasePager()->setUnrefImageDataAfterApplyPolicy(true, false);

        // thread-safe initialization of the OSG wrapper manager. Calling this here
        // prevents the "unsupported wrapper" messages from OSG
        osgDB::Registry::instance()->getObjectWrapperManager()->findWrapper("osg::Image");

        // install our default manipulator (do this before calling load)
        widget.getOsgViewer()->setCameraManipulator(new osgEarth::EarthManipulator(arguments));

        // disable the small-feature culling
        widget.getOsgViewer()->getCamera()->setSmallFeatureCullingPixelSize(-1.0f);

        // load an earth file, and support all or our example command-line options
        auto node = osgEarth::Util::MapNodeHelper().load(arguments, widget.getOsgViewer());
        if (node.valid()) {
            widget.getOsgViewer()->setSceneData(node);

            if (!osgEarth::Util::MapNode::get(node)) {
                // not an earth file? Just view as a normal OSG node or image
                widget.getOsgViewer()->setCameraManipulator(new osgGA::TrackballManipulator);
                osgUtil::Optimizer opt;
                opt.optimize(node, osgUtil::Optimizer::INDEX_MESH);
                osgEarth::Util::ShaderGenerator gen;
                node->accept(gen);
            }
        }
        //        widget.getOsgViewer()->realize();

        return 0;
    });


    widget.show();

    return app.exec();
}
