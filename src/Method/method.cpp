/*
 * src/Method/method.cpp
 * 
 * Author: Zex <top_zlynch@yahoo.com>
 */
#include "Method/method.h"

namespace EasySip
{
	Method::Method()
	: run_(true)
	{
		HeaderFields::init_allowed_fields();
		init_allowed_methods();
		init_allowed_responses();
	}

	Method::~Method()
	{
	}

	void Method::init_allowed_methods()
	{
		allowed_methods_.insert(METHOD_INVITE);
		allowed_methods_.insert(METHOD_CANCEL);
		allowed_methods_.insert(METHOD_ACK);
		allowed_methods_.insert(METHOD_BYE);
		allowed_methods_.insert(METHOD_REGISTER);
		allowed_methods_.insert(METHOD_OPTIONS);
		allowed_methods_.insert(METHOD_SUBSCRIBE);
		allowed_methods_.insert(METHOD_NOTIFY);
		allowed_methods_.insert(METHOD_MESSAGE);
		allowed_methods_.insert(METHOD_INFO);
		allowed_methods_.insert(METHOD_UPDATE);
		allowed_methods_.insert(METHOD_REFER);
		allowed_methods_.insert(METHOD_PRACK);
	}

	void Method::init_allowed_responses()
	{
		allowed_responses_.insert(SIP_RESPONSE_TRYING);
		allowed_responses_.insert(SIP_RESPONSE_RINGING);
		allowed_responses_.insert(SIP_RESPONSE_FORWARDING);
		allowed_responses_.insert(SIP_RESPONSE_QUEUED);
		allowed_responses_.insert(SIP_RESPONSE_SESSION_PROGRESS);
		allowed_responses_.insert(SIP_RESPONSE_SUCCESSFUL);
		allowed_responses_.insert(SIP_RESPONSE_ACCEPTED);
		allowed_responses_.insert(SIP_RESPONSE_MULTI_CHOICES);
		allowed_responses_.insert(SIP_RESPONSE_MOVE_PERM);
		allowed_responses_.insert(SIP_RESPONSE_MOVE_TEMP);
		allowed_responses_.insert(SIP_RESPONSE_USE_PROXY);
		allowed_responses_.insert(SIP_RESPONSE_ALTER_SERVICE);
		allowed_responses_.insert(SIP_RESPONSE_BAD_REQUEST);
		allowed_responses_.insert(SIP_RESPONSE_UNAUTHORIZED);
		allowed_responses_.insert(SIP_RESPONSE_REQUIRE_PAYMENT);
		allowed_responses_.insert(SIP_RESPONSE_FORBIDDEN);
		allowed_responses_.insert(SIP_RESPONSE_NOT_FOUND);
		allowed_responses_.insert(SIP_RESPONSE_METHOD_NOT_ALLOWED);
		allowed_responses_.insert(SIP_RESPONSE_NOT_ACCEPTABLE);
		allowed_responses_.insert(SIP_RESPONSE_REQUIRE_PROXY_AUTHENTICATION);
		allowed_responses_.insert(SIP_RESPONSE_REQUIRE_REQUEST_TIMEOUT);
		allowed_responses_.insert(SIP_RESPONSE_RESOURCE_NOT_AVAIL);
		allowed_responses_.insert(SIP_RESPONSE_REQUEST_ENTITY_TOO_LARGE);
		allowed_responses_.insert(SIP_RESPONSE_REQUEST_URI_TOO_LONG);
		allowed_responses_.insert(SIP_RESPONSE_UNSUPPORTED_MEDIA_TYPE);
		allowed_responses_.insert(SIP_RESPONSE_UNSUPPORTED_URI_SCHEME);
		allowed_responses_.insert(SIP_RESPONSE_BAD_EXTENSION);
		allowed_responses_.insert(SIP_RESPONSE_REQUIRE_EXTENSION);
		allowed_responses_.insert(SIP_RESPONSE_INTERVAL_TOO_BRIEF);
		allowed_responses_.insert(SIP_RESPONSE_UNAVAIL_TEMP);
		allowed_responses_.insert(SIP_RESPONSE_CALL_OR_TRANSACTION_NOT_EXIST);
		allowed_responses_.insert(SIP_RESPONSE_LOOP_DETECTED);
		allowed_responses_.insert(SIP_RESPONSE_TOO_MANY_HOPS);
		allowed_responses_.insert(SIP_RESPONSE_ADDRESS_INCOMPLETE);
		allowed_responses_.insert(SIP_RESPONSE_AMBIGUOUS_URI);
		allowed_responses_.insert(SIP_RESPONSE_BUSY);
		allowed_responses_.insert(SIP_RESPONSE_REQUEST_TERMINATED);
		allowed_responses_.insert(SIP_RESPONSE_NOT_ACCEPTABLE_HERE);
		allowed_responses_.insert(SIP_RESPONSE_REQUEST_PENDING);
		allowed_responses_.insert(SIP_RESPONSE_UNDECIPHERABLE);
		allowed_responses_.insert(SIP_RESPONSE_SERVER_INTERNAL_ERROR);
		allowed_responses_.insert(SIP_RESPONSE_FUNC_NOT_IMPLEMENTED);
		allowed_responses_.insert(SIP_RESPONSE_BAD_GATEWAY);
		allowed_responses_.insert(SIP_RESPONSE_SERVICE_UNAVAIL);
		allowed_responses_.insert(SIP_RESPONSE_SERVICE_TIMEOUT);
		allowed_responses_.insert(SIP_RESPONSE_UNSUPPORTED_VERSION);
		allowed_responses_.insert(SIP_RESPONSE_MESSAGE_TOO_LARGE);
		allowed_responses_.insert(SIP_RESPONSE_GLOBAL_BUSY);
		allowed_responses_.insert(SIP_RESPONSE_CALLEE_DECLINE);
		allowed_responses_.insert(SIP_RESPONSE_GLOBAL_NOT_EXIST);
		allowed_responses_.insert(SIP_RESPONSE_GLOBAL_NOT_ACCEPTABLE);
	}

	void Method::send_msg(RequestMessage &msg)
	{
		udp_.send_buffer(msg.create().Msg());
	}

	void Method::send_msg(ResponseMessage &msg)
	{
		udp_.send_buffer(msg.create().Msg());
	}

	int Method::on_receive_message(std::string &msg)
	{
		int ret;

		if (METHOD_INVITE.code() <= (ret = Message::get_method_from_buffer(allowed_methods_, msg)))
		{
			return on_receive_req(msg, ret);
		}

		if (SIP_RESPONSE_TRYING.code() <= (ret = Message::get_response_code_from_buffer(allowed_responses_, msg)))
		{
			return on_receive_resp(msg, ret);
		}

		//TODO throw exception ??
		return -1;
	}

	int Method::on_receive_req(std::string &msg, const int code)
	{
		RequestMessage in_msg(msg);
		in_msg.parse();

		if (METHOD_ID_INVITE != code)
		{
			Dialog dialog(in_msg);

			if (!dialogs_[dialog.id()])
			{
				// TODO: configurable reject/accept
				if (true)
				{
					ResponseMessage rep(in_msg);
					rep.SipVersion(SIP_VERSION_2_0);
					rep.ResponseCode(SIP_RESPONSE_CALL_OR_TRANSACTION_NOT_EXIST);

					send_msg(rep);
				}
				else
				{
					// TODO: restruct dialog
				}
			}
		}

//		return it->Method()(in_msg);
		switch (code)
		{
			case METHOD_ID_INVITE:
			{
				return on_invite_request(in_msg);
			}
			case METHOD_ID_CANCEL:
			{
				return on_cancel_request(in_msg);
			}
			case METHOD_ID_ACK:
			{
				return on_ack_request(in_msg);
			}
			case METHOD_ID_BYE:
			{
				return on_bye_request(in_msg);
			}
			case METHOD_ID_REGISTER:
			{
				return on_register_request(in_msg);
			}
			case METHOD_ID_OPTIONS:
			{
				return on_options_request(in_msg);
			}
			case METHOD_ID_SUBSCRIBE:
			{
				return on_subscribe_request(in_msg);
			}
			case METHOD_ID_NOTIFY:
			{
				return on_notify_request(in_msg);
			}
			case METHOD_ID_MESSAGE:
			{
				return on_message_request(in_msg);
			}
			case METHOD_ID_INFO:
			{
				return on_info_request(in_msg);
			}
			case METHOD_ID_UPDATE:
			{
				return on_update_request(in_msg);
			}
			case METHOD_ID_REFER:
			{
				return on_refer_request(in_msg);
			}
			case METHOD_ID_PRACK:
			{
				return on_prack_request(in_msg);
			}
			default:
			{
				std::cerr << "Unexpected request: " << code << '\n';
			}
		}

		return 0;
	}

	int Method::on_receive_resp(std::string &msg, const int code)
	{
		ResponseMessage in_msg(msg);
		in_msg.parse();

		return 0;
	}

	int Method::start()
	{
		try
		{
			while (run_)
			{
				if (0 > udp_.recv_buffer(0)) break;
				// TODO: log peer
				std::cout << "peer: <" << udp_.Addr() << ":" << udp_.Port() << ">\n";
				std::string msg(udp_.Message());
				udp_.clear_msg();
				on_receive_message(msg);
			}
		}
		catch (std::exception e)
		{
			std::cout << "exception: " << e.what() << '\n';
			// TODO: log it
		}

		return 0;
	}

	int Method::invite_request()
	{
		InviteMessage req;

		req.SipVersion(SIP_VERSION_2_0);
		req.RequestURI(udp_.Addr());

		req.add_contact()
		->add_uri("sip:zex@"+udp_.SelfAddr());

		req.add_from()
		->add_name("zex")
		.add_uri("sip:zex@"+udp_.SelfAddr())
		.add_param("tag", "293!hsj@df");

		req.add_to()
		->add_name("\"Big Boss\"")
		.add_uri("sip:bigboss@paris.agg.oo");

		req.add_cseq()
		->add_seq("1")
		.add_method(req.Method());

		req.add_via()
		->add_proto(SIP_VERSION_2_0_UDP)
		.add_sentby(udp_.SelfAddr());

		req.add_call_id()
		->add_id("sundo@1311bili");

		if (false /*is_sips(req.req_line_.request_uri_) */
		|| false /*is_sips(req.req_line_.request_uri_) */)
		{
			req.add_contact()->add_uri("sips:utoc@ir.cx");
		}

		send_msg(req);
//		msgq_.push(req.Msg());
//---------------------------------------------------------------
		if (0 > udp_.recv_buffer(0)) return 0;

		ResponseMessage in_msg(udp_.Message());
		in_msg.parse();

		if (in_msg.is_2xx_resp())
		{
			Dialog dialog(in_msg);
			dialogs_.create_dialog(dialog);
	
			DialogId id = dialogs_.last()->id();
			std::cout << *dialogs_[id];

			udp_.send_buffer(AckMessage(in_msg).create().Msg());
		}
		else if (in_msg.is_1xx_resp())
		{
			std::cout << "peer saids: " << in_msg << '\n';
		}
		else
		{
			// TODO: invite req failed feedback
			std::cerr << "Unable to establish session due to "
				<< in_msg.ResponseCode() << "\n";
		}

		return 0;
	}

	int Method::register_request()
	{
		return 0;
	}

	int Method::bye_request()
	{
		ByeMessage req;

		req.SipVersion(SIP_VERSION_2_0);

		if (dialogs_.size())
		{
			req.add_to()
			->add_name("Big Boss\"")
			.add_uri(dialogs_.last()->remote_uri());

			if (dialogs_.last()->id().remote_tag().size())
				req.to_.last()->add_param("tag", dialogs_.last()->id().remote_tag());

			req.add_from()
			->add_name("zex")
			.add_uri(dialogs_.last()->local_uri());

			if (dialogs_.last()->id().local_tag().size())
				req.from_.last()->add_param("tag", dialogs_.last()->id().local_tag());

			req.add_call_id()
			->add_id(dialogs_.last()->id().call_id().id_);

			std::string seq;
	
			if (!dialogs_.last()->local_seq().cseq_.empty())
			{
				dialogs_.last()->local_seq().inc_seq();
				seq = dialogs_.last()->local_seq().cseq_;
			}

			if (seq.empty())
			{
				seq = "1"; // TODO: choose a seq, 32bits
			}
	
			req.add_cseq()
			->add_seq(seq)
			.add_method(req.Method());

			if (dialogs_.last()->remote_target().size())
				req.RequestURI(dialogs_.last()->remote_target().last()->uri());

			if (dialogs_.last()->routes().size())
			{
				if (dialogs_.last()->routes().last()->cons_.last()->has_param("lr"))
				{
//					if (dialogs_.last()->remote_target().size())
					req.RequestURI(dialogs_.last()->remote_target().last()->uri());

					req.add_route();

					if (dialogs_.last()->routes().size())
					{
						req.route_.last()->cons_ = dialogs_.last()->routes().last()->cons_;
					}
				}
				else
				{
					req.RequestURI(dialogs_.last()->routes().last()->cons_.last()->uri());

					req.add_route();

					ContactList::iterator from = dialogs_.last()->routes().last()->cons_.begin();
					from++;

					req.route_.last()->cons_.append(from, dialogs_.last()->routes().last()->cons_.end());
					req.route_.last()->cons_.append(dialogs_.last()->remote_target());
				}
			}
		}

		req.add_via()
		->add_proto(SIP_VERSION_2_0_UDP)
		.add_sentby(udp_.SelfAddr());


		if (false /*is_sips(req.req_line_.request_uri_) */
		|| false /*is_sips(req.req_line_.request_uri_) */)
		{
			req.add_contact()->add_uri("sips:utoc@ir.cx");
		}

		send_msg(req);
//		msgq_.push(req.Msg());
//---------------------------------------------------------------
//		if (0 > udp_.recv_buffer(0)) return 0;
//		ResponseMessage in_msg(udp_.Message());
//		in_msg.parse();

		return 0;
	}

	int Method::cancel_request()
	{
		return 0;
	}

	int Method::update_request()
	{
		return 0;
	}

	int Method::info_request()
	{
		return 0;
	}

	int Method::ack_request()
	{
		AckMessage req;

		req.SipVersion(SIP_VERSION_2_0);

		if (dialogs_.size())
		{
			req.add_to()
			->add_name("Big Boss\"")
			.add_uri(dialogs_.last()->remote_uri());

			if (dialogs_.last()->id().remote_tag().size())
				req.to_.last()->add_param("tag", dialogs_.last()->id().remote_tag());

			req.add_from()
			->add_name("zex")
			.add_uri(dialogs_.last()->local_uri());

			if (dialogs_.last()->id().local_tag().size())
				req.from_.last()->add_param("tag", dialogs_.last()->id().local_tag());

			req.add_call_id()
			->add_id(dialogs_.last()->id().call_id().id_);

			std::string seq;
	
			if (!dialogs_.last()->local_seq().cseq_.empty())
			{
				dialogs_.last()->local_seq().inc_seq();
				seq = dialogs_.last()->local_seq().cseq_;
			}

			if (seq.empty())
			{
				seq = "1"; // TODO: choose a seq, 32bits
			}
	
			req.add_cseq()
			->add_seq(seq)
			.add_method(req.Method());

			if (dialogs_.last()->remote_target().size())
				req.RequestURI(dialogs_.last()->remote_target().last()->uri());

			if (dialogs_.last()->routes().size())
			{
				if (dialogs_.last()->routes().last()->cons_.last()->has_param("lr"))
				{
//					if (dialogs_.last()->remote_target().size())
					req.RequestURI(dialogs_.last()->remote_target().last()->uri());

					req.add_route();

					if (dialogs_.last()->routes().size())
					{
						req.route_.last()->cons_ = dialogs_.last()->routes().last()->cons_;
					}
				}
				else
				{
					req.RequestURI(dialogs_.last()->routes().last()->cons_.last()->uri());

					req.add_route();

					ContactList::iterator from = dialogs_.last()->routes().last()->cons_.begin();
					from++;

					req.route_.last()->cons_.append(from, dialogs_.last()->routes().last()->cons_.end());
					req.route_.last()->cons_.append(dialogs_.last()->remote_target());
				}
			}
		}

		req.add_via()
		->add_proto(SIP_VERSION_2_0_UDP)
		.add_sentby(udp_.SelfAddr());


		if (false /*is_sips(req.req_line_.request_uri_) */
		|| false /*is_sips(req.req_line_.request_uri_) */)
		{
			req.add_contact()->add_uri("sips:utoc@ir.cx");
		}

		send_msg(req);
		return 0;
	}

	int Method::message_request()
	{
		return 0;
	}

	int Method::subscribe_request()
	{
		return 0;
	}

	int Method::notify_request()
	{
		return 0;
	}

	int Method::refer_request()
	{
		return 0;
	}

	int Method::options_request()
	{
		OptionsMessage req;

		req.SipVersion(SIP_VERSION_2_0);

		if (dialogs_.size())
		{
			req.add_to()
			->add_name("Big Boss\"")
			.add_uri(dialogs_.last()->remote_uri());

			if (dialogs_.last()->id().remote_tag().size())
				req.to_.last()->add_param("tag", dialogs_.last()->id().remote_tag());

			req.add_from()
			->add_name("zex")
			.add_uri(dialogs_.last()->local_uri());

			if (dialogs_.last()->id().local_tag().size())
				req.from_.last()->add_param("tag", dialogs_.last()->id().local_tag());

			req.add_call_id()
			->add_id(dialogs_.last()->id().call_id().id_);

			std::string seq;
	
			if (!dialogs_.last()->local_seq().cseq_.empty())
			{
				dialogs_.last()->local_seq().inc_seq();
				seq = dialogs_.last()->local_seq().cseq_;
			}

			if (seq.empty())
			{
				seq = "1"; // TODO: choose a seq, 32bits
			}
	
			req.add_cseq()
			->add_seq(seq)
			.add_method(req.Method());

			if (dialogs_.last()->remote_target().size())
				req.RequestURI(dialogs_.last()->remote_target().last()->uri());

			if (dialogs_.last()->routes().size())
			{
				if (dialogs_.last()->routes().last()->cons_.last()->has_param("lr"))
				{
//					if (dialogs_.last()->remote_target().size())
					req.RequestURI(dialogs_.last()->remote_target().last()->uri());

					req.add_route();

					if (dialogs_.last()->routes().size())
					{
						req.route_.last()->cons_ = dialogs_.last()->routes().last()->cons_;
					}
				}
				else
				{
					req.RequestURI(dialogs_.last()->routes().last()->cons_.last()->uri());

					req.add_route();

					ContactList::iterator from = dialogs_.last()->routes().last()->cons_.begin();
					from++;

					req.route_.last()->cons_.append(from, dialogs_.last()->routes().last()->cons_.end());
					req.route_.last()->cons_.append(dialogs_.last()->remote_target());
				}
			}
		}

		req.add_via()
		->add_proto(SIP_VERSION_2_0_UDP)
		.add_sentby(udp_.SelfAddr());


		if (false /*is_sips(req.req_line_.request_uri_) */
		|| false /*is_sips(req.req_line_.request_uri_) */)
		{
			req.add_contact()->add_uri("sips:utoc@ir.cx");
		}

		send_msg(req);
//		msgq_.push(req.Msg());
//---------------------------------------------------------------
		if (0 > udp_.recv_buffer(0)) return 0;
		ResponseMessage in_msg(udp_.Message());
		in_msg.parse();

		return 0;
	}

	int Method::prack_request()
	{
		return 0;
	}

	int Method::on_invite_request(RequestMessage &in_msg)
	{
		ResponseMessage rep(in_msg);

		rep.SipVersion(SIP_VERSION_2_0);
		rep.ResponseCode(SIP_RESPONSE_RINGING);

		rep.add_contact()
		->add_uri("sip:ag@"+udp_.Addr());

		if (in_msg.record_route_.size())
			rep.record_route_ = in_msg.record_route_;

		Dialog dialog(in_msg);
		dialogs_.create_dialog(dialog);

		std::cout << "----------\n" << *dialogs_.last() << "-----------\n";

		send_msg(rep);

		dialogs_[dialog.id()]->still_ringing(true);
		// TODO: timeout here
		for (int i = 3; i; i--)
		{
			std::cout << "ringing ... ...\n";
//			sleep(2);
		}

		rep.ResponseCode(SIP_RESPONSE_SUCCESSFUL);
		send_msg(rep);

		// TODO: timeout here for ACK

		return 0;
	}
	
	int Method::on_register_request(RequestMessage &in_msg)
	{
//		ResponseMessage rep(in_msg);
//		rep.SipVersion(SIP_VERSION_2_0);
//		rep.ResponseCode(SIP_RESPONSE_TRYING);
//
//		rep.via_.last()->HeaderParam("received", udp_.Addr());
////		rep.append_userdata("top of the hill");
//

		return 0;
	}
	
	int Method::on_bye_request(RequestMessage &in_msg)
	{
		ResponseMessage rep(in_msg);
	
		Dialog dialog(in_msg);

//		if (in_msg.record_route_.size())
//			rep.record_route_ = in_msg.record_route_;

		dialogs_.cancel_dialog(dialog.id());

		return 0;
	}
	
	int Method::on_cancel_request(RequestMessage &in_msg)
	{
		Dialog dialog(in_msg);

		if (dialogs_[dialog.id()])
		{
			if (dialogs_[dialog.id()]->still_ringing())
			{
				// TODO: cancel it
			}
			else
			{
				ResponseMessage rep(in_msg);

				rep.SipVersion(SIP_VERSION_2_0);
				rep.ResponseCode(SIP_RESPONSE_REQUEST_TERMINATED);

				send_msg(rep);
			}
		}

		return 0;
	}
	
	int Method::on_ack_request(RequestMessage &in_msg)
	{
		Dialog dialog(in_msg);

		if (dialogs_[dialog.id()])
		{
			dialogs_[dialog.id()]->is_confirmed(true);
		}

		return 0;
	}
	
	int Method::on_options_request(RequestMessage &in_msg)
	{
		ResponseMessage rep(in_msg);

		rep.SipVersion(SIP_VERSION_2_0);
		rep.ResponseCode(SIP_RESPONSE_SUCCESSFUL);

		rep.add_accept()
		->add_value("text", "plain")
		.add_value("text", "html")
		.add_value("application", "sdp")
		.add_value("multipart", "sdp");

		rep.add_allow();

		for (auto &it : allowed_methods_) 
			rep.allow_.last()->add_value(it.name());

		udp_.send_buffer(rep.create().Msg());

		return 0;
	}
	
	int Method::on_subscribe_request(RequestMessage &in_msg)
	{
		std::cout << __PRETTY_FUNCTION__ << '\n';
		return 0;
	}
	
	int Method::on_notify_request(RequestMessage &in_msg)
	{
		std::cout << __PRETTY_FUNCTION__ << '\n';
		return 0;
	}
	
	int Method::on_info_request(RequestMessage &in_msg)
	{
		std::cout << __PRETTY_FUNCTION__ << '\n';
		return 0;
	}
	
	int Method::on_update_request(RequestMessage &in_msg)
	{
		std::cout << __PRETTY_FUNCTION__ << '\n';
		return 0;
	}
	
	int Method::on_refer_request(RequestMessage &in_msg)
	{
		std::cout << __PRETTY_FUNCTION__ << '\n';
		return 0;
	}
	
	int Method::on_message_request(RequestMessage &in_msg)
	{
		std::cout << __PRETTY_FUNCTION__ << '\n';
		return 0;
	}
	
	int Method::on_prack_request(RequestMessage &in_msg)
	{
		std::cout << __PRETTY_FUNCTION__ << '\n';
		return 0;
	}

} // namespace EasySip
