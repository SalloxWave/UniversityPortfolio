package ChatApp;


/**
* ChatApp/_ChatStub.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from Chat.idl
* Monday, 27 March 2017 15:07:03 o'clock CEST
*/

public class _ChatStub extends org.omg.CORBA.portable.ObjectImpl implements ChatApp.Chat
{

  public void join (ChatApp.ChatCallback objref, String userName)
  {
            org.omg.CORBA.portable.InputStream $in = null;
            try {
                org.omg.CORBA.portable.OutputStream $out = _request ("join", true);
                ChatApp.ChatCallbackHelper.write ($out, objref);
                $out.write_string (userName);
                $in = _invoke ($out);
                return;
            } catch (org.omg.CORBA.portable.ApplicationException $ex) {
                $in = $ex.getInputStream ();
                String _id = $ex.getId ();
                throw new org.omg.CORBA.MARSHAL (_id);
            } catch (org.omg.CORBA.portable.RemarshalException $rm) {
                join (objref, userName        );
            } finally {
                _releaseReply ($in);
            }
  } // join

  public void leave (ChatApp.ChatCallback objref)
  {
            org.omg.CORBA.portable.InputStream $in = null;
            try {
                org.omg.CORBA.portable.OutputStream $out = _request ("leave", true);
                ChatApp.ChatCallbackHelper.write ($out, objref);
                $in = _invoke ($out);
                return;
            } catch (org.omg.CORBA.portable.ApplicationException $ex) {
                $in = $ex.getInputStream ();
                String _id = $ex.getId ();
                throw new org.omg.CORBA.MARSHAL (_id);
            } catch (org.omg.CORBA.portable.RemarshalException $rm) {
                leave (objref        );
            } finally {
                _releaseReply ($in);
            }
  } // leave

  public void post (ChatApp.ChatCallback objref, String message)
  {
            org.omg.CORBA.portable.InputStream $in = null;
            try {
                org.omg.CORBA.portable.OutputStream $out = _request ("post", true);
                ChatApp.ChatCallbackHelper.write ($out, objref);
                $out.write_string (message);
                $in = _invoke ($out);
                return;
            } catch (org.omg.CORBA.portable.ApplicationException $ex) {
                $in = $ex.getInputStream ();
                String _id = $ex.getId ();
                throw new org.omg.CORBA.MARSHAL (_id);
            } catch (org.omg.CORBA.portable.RemarshalException $rm) {
                post (objref, message        );
            } finally {
                _releaseReply ($in);
            }
  } // post

  public void list (ChatApp.ChatCallback objref)
  {
            org.omg.CORBA.portable.InputStream $in = null;
            try {
                org.omg.CORBA.portable.OutputStream $out = _request ("list", true);
                ChatApp.ChatCallbackHelper.write ($out, objref);
                $in = _invoke ($out);
                return;
            } catch (org.omg.CORBA.portable.ApplicationException $ex) {
                $in = $ex.getInputStream ();
                String _id = $ex.getId ();
                throw new org.omg.CORBA.MARSHAL (_id);
            } catch (org.omg.CORBA.portable.RemarshalException $rm) {
                list (objref        );
            } finally {
                _releaseReply ($in);
            }
  } // list

  public void gomoku (ChatApp.ChatCallback objref, String color)
  {
            org.omg.CORBA.portable.InputStream $in = null;
            try {
                org.omg.CORBA.portable.OutputStream $out = _request ("gomoku", true);
                ChatApp.ChatCallbackHelper.write ($out, objref);
                $out.write_string (color);
                $in = _invoke ($out);
                return;
            } catch (org.omg.CORBA.portable.ApplicationException $ex) {
                $in = $ex.getInputStream ();
                String _id = $ex.getId ();
                throw new org.omg.CORBA.MARSHAL (_id);
            } catch (org.omg.CORBA.portable.RemarshalException $rm) {
                gomoku (objref, color        );
            } finally {
                _releaseReply ($in);
            }
  } // gomoku

  public void add (ChatApp.ChatCallback objref, int x, int y)
  {
            org.omg.CORBA.portable.InputStream $in = null;
            try {
                org.omg.CORBA.portable.OutputStream $out = _request ("add", true);
                ChatApp.ChatCallbackHelper.write ($out, objref);
                $out.write_long (x);
                $out.write_long (y);
                $in = _invoke ($out);
                return;
            } catch (org.omg.CORBA.portable.ApplicationException $ex) {
                $in = $ex.getInputStream ();
                String _id = $ex.getId ();
                throw new org.omg.CORBA.MARSHAL (_id);
            } catch (org.omg.CORBA.portable.RemarshalException $rm) {
                add (objref, x, y        );
            } finally {
                _releaseReply ($in);
            }
  } // add

  // Type-specific CORBA::Object operations
  private static String[] __ids = {
    "IDL:ChatApp/Chat:1.0"};

  public String[] _ids ()
  {
    return (String[])__ids.clone ();
  }

  private void readObject (java.io.ObjectInputStream s) throws java.io.IOException
  {
     String str = s.readUTF ();
     String[] args = null;
     java.util.Properties props = null;
     org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init (args, props);
   try {
     org.omg.CORBA.Object obj = orb.string_to_object (str);
     org.omg.CORBA.portable.Delegate delegate = ((org.omg.CORBA.portable.ObjectImpl) obj)._get_delegate ();
     _set_delegate (delegate);
   } finally {
     orb.destroy() ;
   }
  }

  private void writeObject (java.io.ObjectOutputStream s) throws java.io.IOException
  {
     String[] args = null;
     java.util.Properties props = null;
     org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init (args, props);
   try {
     String str = orb.object_to_string (this);
     s.writeUTF (str);
   } finally {
     orb.destroy() ;
   }
  }
} // class _ChatStub
