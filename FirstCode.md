1.Activity

* match_parent 表示让当前元素和父元素一样

* 用wrap_content 表示当前元素的高度只要能刚好包含里面的内容就行
* setContentView() 方法来给当前的活动加载一个布局，而 在setContentView() 方法中，我们一般都会传入一个布局文件的id
* 所有的活动都要在AndroidManifest.xml中进行注册才能生效
* 仅仅是这样注册了活动，我们的程序仍然是不能运行的，因为还没有为程序配置主活动，也就是说，当程序运行起来的时候，不知道要首先启动哪个活动。配置主活动的方法其实在第1章中已经介绍过了，就是在<activity> 标签的内部加入<intent-filter> 标签，并在这个标签里添加<action android:name="android.intent.action.MAIN"/> 和 <category android: name="android.intent.category.LAUNCHER" /> 这两句声 明即可。 
* 过findViewById() 方法获取到在布局文件中定义的元素
* 通过getMenuInflater() 方法能够得到MenuInflater 对象，再调用它的inflate() 方法 就可以给当前活动创建菜单了。inflate() 方法接收两个参数，第一个参数用于指定我们通过 哪一个资源文件来创建菜单，这里当然传入R.menu.main 。第二个参数用于指定我们的菜单 项将添加到哪一个Menu 对象当中，这里直接使用onCreateOptionsMenu() 方法中传入的 menu 参数。然后给这个方法返回true ，表示允许创建的菜单显示出来，如果返回了false，创建的菜单将无法显示。 
* 只要按一下Back键就可以销毁当前的活动了。不过如果你不想通过按键的 方式，而是希望在程序中通过代码来销毁活动，当然也可以，Activity类提供了一个finish() 方法，我们在活动中调用一下这个方法就可以销毁当前活动了。 
* 由于SecondActivity不是主活动，因此不需要配置<intent-filter> 标签里的内容，注册活动 的代码也简单了许多。
* Intent是Android程序中各组件之间进行交互的一种重要方式，它不仅可以指明当前组件想要执行 的动作，还可以在不同组件之间传递数据。Intent一般可被用于启动活动、启动服务以及发送广播等场景，由于服务、广播等概念你暂时还未涉及，那么本章我们的目光无疑就锁定在了启动 活动上面。 
* Intent有多个构造函数的重载，其中一个是Intent(Context packageContext, Class<?> cls) 。这个构造函数接收两个参数，第一个参数Context 要求提供一个启动活动的上下文， 第二个参数Class 则是指定想要启动的目标活动，通过这个构造函数就可以构建出Intent 的“意图”。然后我们应该怎么使用这个Intent呢？Activity类中提供了一个startActivity() 方法，这个方法是专门用于启动活动的，它接收一个Intent 参数，这里我们将构建好的Intent传 入startActivity() 方法就可以启动目标活动了。
* 相比于显式Intent，隐式Intent则含蓄了许多，它并不明确指出我们想要启动哪一个活动，而是指 定了一系列更为抽象的action 和category 等信息，然后交由系统去分析这个Intent，并帮我 们找出合适的活动去启动。
* 使用隐式Intent，我们不仅可以启动自己程序内的活动，还可以启动其他程序的活动，这使得 Android多个应用程序之间的功能共享成为了可能。比如说你的应用程序中需要展示一个网页， 这时你没有必要自己去实现一个浏览器（事实上也不太可能），而是只需要调用系统的浏览器来打开这个网页就行了。  
* 不过到目前为止，我们都只是简单地使 用Intent来启动一个活动，其实Intent还可以在启动活动的时候传递数据，下面我们来一起看一 下。