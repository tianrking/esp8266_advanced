lib/
  └── webserver/
      ├── webserver.h              # 主头文件
      ├── webserver.cpp            # 主实现文件
      ├── pages/                   # 页面实现目录
      │   ├── page_home.h         # 主页
      │   ├── page_home.cpp
      │   ├── page_control.h      # 控制页面
      │   ├── page_control.cpp
      │   ├── page_sensor.h       # 传感器页面
      │   ├── page_sensor.cpp
      │   ├── page_mode.h         # 运动模式页面
      │   ├── page_mode.cpp
      │   ├── page_log.h          # 日志页面
      │   ├── page_log.cpp
      │   ├── page_settings.h     # 设置页面
      │   ├── page_settings.cpp
      │   ├── page_advanced.h     # 高级设置页面
      │   ├── page_advanced.cpp
      │   ├── page_debug.h        # 调试页面
      │   └── page_debug.cpp
      ├── api/                     # API实现目录
      │   ├── api_control.h       # 控制相关API
      │   ├── api_control.cpp
      │   ├── api_sensor.h        # 传感器相关API
      │   ├── api_sensor.cpp
      │   ├── api_log.h           # 日志相关API
      │   ├── api_log.cpp
      │   ├── api_settings.h      # 设置相关API
      │   └── api_settings.cpp
      └── common/                  # 公共组件目录
          ├── html_common.h        # 公共HTML组件
          ├── html_common.cpp
          ├── style_common.h       # 公共样式
          └── style_common.cpp