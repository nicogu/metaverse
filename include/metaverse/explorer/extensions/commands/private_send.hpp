/**
 * Copyright (c) 2016-2017 mvs developers 
 *
 * This file is part of metaverse-explorer.
 *
 * metaverse-explorer is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include <metaverse/bitcoin.hpp>
#include <metaverse/explorer/define.hpp>
#include <metaverse/explorer/command.hpp>
#include <metaverse/explorer/extensions/command_extension.hpp>

namespace libbitcoin {
namespace explorer {
namespace commands {

/************************ deposit *************************/

class deposit : public command_extension
{
public:
    static const char* symbol(){ return "deposit";}
    const char* name() override { return symbol();} 
    const char* category() override { return "EXTENSION"; }
    const char* description() override { return "Deposit some etp, then get reward for frozen some etp."; }

    arguments_metadata& load_arguments() override
    {
        return get_argument_metadata()
            .add("ACCOUNTNAME", 1)
            .add("ACCOUNTAUTH", 1)
            .add("AMOUNT", 1);
    }

    void load_fallbacks (std::istream& input, 
        po::variables_map& variables) override
    {
        const auto raw = requires_raw_input();
        load_input(auth_.name, "ACCOUNTNAME", variables, input, raw);
        load_input(auth_.auth, "ACCOUNTAUTH", variables, input, raw);
        load_input(argument_.amount, "AMOUNT", variables, input, raw);
    }

    options_metadata& load_options() override
    {
        using namespace po;
        options_description& options = get_option_metadata();
        options.add_options()
		(
            BX_HELP_VARIABLE ",h",
            value<bool>()->zero_tokens(),
            "Get a description and instructions for this command."
        )
        (
            BX_CONFIG_VARIABLE ",c",
            value<boost::filesystem::path>(),
            "The path to the configuration settings file."
        )
	    (
            "ACCOUNTNAME",
            value<std::string>(&auth_.name)->required(),
            "Account name."
	    )
        (
            "ACCOUNTAUTH",
            value<std::string>(&auth_.auth)->required(),
            "Account password/authorization."
	    )
        (
            "AMOUNT",
            value<uint64_t>(&argument_.amount)->required(),
            "How many you will deposit."
        )
		(
			"address,a",
			value<std::string>(&argument_.address),
			"The deposit target address."
		)
	    (
            "deposit,d",
            value<uint16_t>(&argument_.deposit)->default_value(7),
            "Deposits support [7, 30, 90, 182, 365] days. defaluts to 7 days"
	    )
	    (
            "fee,f",
            value<uint64_t>(&argument_.fee)->default_value(10000),
            "The fee of tx. default_value 0.0001 etp"
	    );

        return options;
    }

    void set_defaults_from_config (po::variables_map& variables) override
    {
    }

    console_result invoke (std::ostream& output,
        std::ostream& cerr, bc::blockchain::block_chain_impl& blockchain) override;

    struct argument
    {
        uint64_t amount;
        uint64_t fee;
        uint16_t deposit;
		std::string address;
    } argument_;

    struct option
    {
    } option_;

};

/************************ send *************************/

class send: public send_command
{
public:
    static const char* symbol(){ return "send";}
    const char* name() override { return symbol();} 
    const char* category() override { return "EXTENSION"; }
    const char* description() override { return "send etp to a targert address, mychange goes to another existed address of this account."; }

    arguments_metadata& load_arguments() override
    {
        return get_argument_metadata()
            .add("ACCOUNTNAME", 1)
            .add("ACCOUNTAUTH", 1)
            .add("TOADDRESS", 1)
            .add("AMOUNT", 1);
    }

    void load_fallbacks (std::istream& input, 
        po::variables_map& variables) override
    {
        const auto raw = requires_raw_input();
        load_input(auth_.name, "ACCOUNTNAME", variables, input, raw);
        load_input(auth_.auth, "ACCOUNTAUTH", variables, input, raw);
        load_input(argument_.address, "TOADDRESS", variables, input, raw);
        load_input(argument_.amount, "AMOUNT", variables, input, raw);
    }

    options_metadata& load_options() override
    {
        using namespace po;
        options_description& options = get_option_metadata();
        options.add_options()
		(
            BX_HELP_VARIABLE ",h",
            value<bool>()->zero_tokens(),
            "Get a description and instructions for this command."
        )
        (
            BX_CONFIG_VARIABLE ",c",
            value<boost::filesystem::path>(),
            "The path to the configuration settings file."
        )
	    (
            "ACCOUNTNAME",
            value<std::string>(&auth_.name)->required(),
            "Account name."
	    )
        (
            "ACCOUNTAUTH",
            value<std::string>(&auth_.auth)->required(),
            "Account password/authorization."
	    )
        (
            "TOADDRESS",
            value<std::string>(&argument_.address)->required(),
            "Send to this address"
	    )
        (
            "AMOUNT",
            value<uint64_t>(&argument_.amount)->required(),
            "How many you will spend"
        )
	    (
            "fee,f",
            value<uint64_t>(&argument_.fee)->default_value(10000),
            "The fee of tx. default_value 0.0001 etp"
	    );

        return options;
    }

    void set_defaults_from_config (po::variables_map& variables) override
    {
    }

    console_result invoke (std::ostream& output,
        std::ostream& cerr, bc::blockchain::block_chain_impl& blockchain) override;

    struct argument
    {
        std::string address;
        uint64_t amount;
        uint64_t fee;
    } argument_;

    struct option
    {
    } option_;

};



/************************ sendmore *************************/

class sendmore: public send_command
{
public:
    static const char* symbol(){ return "sendmore";}
    const char* name() override { return symbol();} 
    const char* category() override { return "EXTENSION"; }
    const char* description() override { return "send etp to multi target addresses, can specify mychange address. Eg: [sendmore $name $password -r $address1:$amount1 -r $address2:$amount2 -m $mychange_address]"; }

    arguments_metadata& load_arguments() override
    {
        return get_argument_metadata()
            .add("ACCOUNTNAME", 1)
            .add("ACCOUNTAUTH", 1);
    }

    void load_fallbacks (std::istream& input, 
        po::variables_map& variables) override
    {
        const auto raw = requires_raw_input();
        load_input(auth_.name, "ACCOUNTNAME", variables, input, raw);
        load_input(auth_.auth, "ACCOUNTAUTH", variables, input, raw);
    }

    options_metadata& load_options() override
    {
        using namespace po;
        options_description& options = get_option_metadata();
        options.add_options()
		(
            BX_HELP_VARIABLE ",h",
            value<bool>()->zero_tokens(),
            "Send to more target. "
        )
        (
            BX_CONFIG_VARIABLE ",c",
            value<boost::filesystem::path>(),
            "Get a description and instructions for this command."
        )
	    (
            "ACCOUNTNAME",
            value<std::string>(&auth_.name)->required(),
            "Account name."
	    )
        (
            "ACCOUNTAUTH",
            value<std::string>(&auth_.auth)->required(),
            "Account password/authorization."
	    )
        (
            "receivers,r",
            value<std::vector<std::string>>(&argument_.receivers)->required(),
            "Send to [address:amount]."
	    )
        (
            "mychange,m",
            value<std::string>(&argument_.mychange_address),
            "Mychange to this address"
	    )
	    (
            "fee,f",
            value<uint64_t>(&argument_.fee)->default_value(10000),
            "The fee of tx. default_value 0.0001 etp"
	    );


        return options;
    }

    void set_defaults_from_config (po::variables_map& variables) override
    {
    }

    console_result invoke (std::ostream& output,
        std::ostream& cerr, bc::blockchain::block_chain_impl& blockchain) override;

    struct argument
    {
        std::vector<std::string> receivers;
        std::string mychange_address;
        uint64_t fee;
    } argument_;

    struct option
    {
    } option_;

};



/************************ sendfrom *************************/

class sendfrom: public send_command
{
public:
    static const char* symbol(){ return "sendfrom";}
    const char* name() override { return symbol();} 
    const char* category() override { return "EXTENSION"; }
    const char* description() override { return "send etp from a specified address of this account to target address, mychange goes to from_address."; }

    arguments_metadata& load_arguments() override
    {
        return get_argument_metadata()
            .add("ACCOUNTNAME", 1)
            .add("ACCOUNTAUTH", 1)
            .add("FROMADDRESS", 1)
            .add("TOADDRESS", 1)
            .add("AMOUNT", 1);
    }

    void load_fallbacks (std::istream& input, 
        po::variables_map& variables) override
    {
        const auto raw = requires_raw_input();
        load_input(auth_.name, "ACCOUNTNAME", variables, input, raw);
        load_input(auth_.auth, "ACCOUNTAUTH", variables, input, raw);
        load_input(auth_.auth, "FROMADDRESS", variables, input, raw);
        load_input(auth_.auth, "TOADDRESS", variables, input, raw);
        load_input(auth_.auth, "AMOUNT", variables, input, raw);
    }

    options_metadata& load_options() override
    {
        using namespace po;
        options_description& options = get_option_metadata();
        options.add_options()
		(
            BX_HELP_VARIABLE ",h",
            value<bool>()->zero_tokens(),
            "Get a description and instructions for this command."
        )
        (
            BX_CONFIG_VARIABLE ",c",
            value<boost::filesystem::path>(),
            "The path to the configuration settings file."
        )
	    (
            "ACCOUNTNAME",
            value<std::string>(&auth_.name)->required(),
            "Account name."
	    )
        (
            "ACCOUNTAUTH",
            value<std::string>(&auth_.auth)->required(),
            "Account password/authorization."
	    )
		(
			"FROMADDRESS",
			value<std::string>(&argument_.from)->required(),
			"Send from this address"
		)
		(
			"TOADDRESS",
			value<std::string>(&argument_.to)->required(),
			"Send to this address"
		)
		(
			"AMOUNT",
			value<uint64_t>(&argument_.amount)->required(),
			"How many you will spend"
		)
		(
			"fee,f",
			value<uint64_t>(&argument_.fee)->default_value(10000),
			"The fee of tx. default_value 0.0001 etp"
		);

        return options;
    }

    void set_defaults_from_config (po::variables_map& variables) override
    {
    }

    console_result invoke (std::ostream& output,
        std::ostream& cerr, bc::blockchain::block_chain_impl& blockchain) override;

    struct argument
    {
    	std::string from;
		std::string to;
		uint64_t amount;
		uint64_t fee;
    } argument_;

    struct option
    {
    } option_;

};

/************************ sendwithmsg *************************/

class sendwithmsg: public send_command
{
public:
    static const char* symbol(){ return "sendwithmsg";}
    const char* name() override { return symbol();} 
    const char* category() override { return "EXTENSION"; }
    const char* description() override { return "sendwithmsg "; }

    arguments_metadata& load_arguments() override
    {
        return get_argument_metadata()
            .add("ACCOUNTNAME", 1)
            .add("ACCOUNTAUTH", 1)
            .add("TOADDRESS", 1)
            .add("AMOUNT", 1)
            .add("MESSAGE", 1);
    }

    void load_fallbacks (std::istream& input, 
        po::variables_map& variables) override
    {
        const auto raw = requires_raw_input();
        load_input(auth_.name, "ACCOUNTNAME", variables, input, raw);
        load_input(auth_.auth, "ACCOUNTAUTH", variables, input, raw);
        load_input(argument_.address, "TOADDRESS", variables, input, raw);
        load_input(argument_.amount, "AMOUNT", variables, input, raw);
        load_input(argument_.message, "MESSAGE", variables, input, raw);
    }

    options_metadata& load_options() override
    {
        using namespace po;
        options_description& options = get_option_metadata();
        options.add_options()
		(
            BX_HELP_VARIABLE ",h",
            value<bool>()->zero_tokens(),
            "Get a description and instructions for this command."
        )
        (
            BX_CONFIG_VARIABLE ",c",
            value<boost::filesystem::path>(),
            "The path to the configuration settings file."
        )
	    (
            "ACCOUNTNAME",
            value<std::string>(&auth_.name)->required(),
            "Account name."
	    )
        (
            "ACCOUNTAUTH",
            value<std::string>(&auth_.auth)->required(),
            "Account password/authorization."
	    )
        (
            "TOADDRESS",
            value<std::string>(&argument_.address)->required(),
            "Send to this address"
	    )
        (
            "AMOUNT",
            value<uint64_t>(&argument_.amount)->required(),
            "How many you will spend"
        )
		(
			"MESSAGE",
			value<std::string>(&argument_.message)->required(),
			"message attached to the transaction"
		)
	    (
            "fee,f",
            value<uint64_t>(&argument_.fee)->default_value(10000),
            "The fee of tx. default_value 0.0001 etp"
	    );

        return options;
    }

    void set_defaults_from_config (po::variables_map& variables) override
    {
    }

    console_result invoke (std::ostream& output,
        std::ostream& cerr, bc::blockchain::block_chain_impl& blockchain) override;

    struct argument
    {
        std::string address;
        uint64_t amount;
		std::string message;
        uint64_t fee;
    } argument_;

    struct option
    {
    } option_;

};


/************************ sendwithmsgfrom *************************/

class sendwithmsgfrom: public send_command
{
public:
    static const char* symbol(){ return "sendwithmsgfrom";}
    const char* name() override { return symbol();} 
    const char* category() override { return "EXTENSION"; }
    const char* description() override { return "sendwithmsgfrom "; }

    arguments_metadata& load_arguments() override
    {
        return get_argument_metadata()
            .add("ACCOUNTNAME", 1)
            .add("ACCOUNTAUTH", 1)
            .add("FROMADDRESS", 1)
            .add("TOADDRESS", 1)
            .add("AMOUNT", 1)
            .add("MESSAGE", 1);
    }

    void load_fallbacks (std::istream& input, 
        po::variables_map& variables) override
    {
        const auto raw = requires_raw_input();
        load_input(auth_.name, "ACCOUNTNAME", variables, input, raw);
        load_input(auth_.auth, "ACCOUNTAUTH", variables, input, raw);
        load_input(argument_.from, "FROMADDRESS", variables, input, raw);
        load_input(argument_.to, "TOADDRESS", variables, input, raw);
        load_input(argument_.amount, "AMOUNT", variables, input, raw);
        load_input(argument_.message, "MESSAGE", variables, input, raw);
    }

    options_metadata& load_options() override
    {
        using namespace po;
        options_description& options = get_option_metadata();
        options.add_options()
		(
            BX_HELP_VARIABLE ",h",
            value<bool>()->zero_tokens(),
            "Get a description and instructions for this command."
        )
        (
            BX_CONFIG_VARIABLE ",c",
            value<boost::filesystem::path>(),
            "The path to the configuration settings file."
        )
	    (
            "ACCOUNTNAME",
            value<std::string>(&auth_.name)->required(),
            "Account name."
	    )
        (
            "ACCOUNTAUTH",
            value<std::string>(&auth_.auth)->required(),
            "Account password/authorization."
	    )
		(
			"FROMADDRESS",
			value<std::string>(&argument_.from)->required(),
			"Send from this address"
		)
		(
			"TOADDRESS",
			value<std::string>(&argument_.to)->required(),
			"Send to this address"
		)
		(
			"AMOUNT",
			value<uint64_t>(&argument_.amount)->required(),
			"How many you will spend"
		)
		(
			"MESSAGE",
			value<std::string>(&argument_.message)->required(),
			"message attached to the transaction"
		)
		(
			"fee,f",
			value<uint64_t>(&argument_.fee)->default_value(10000),
			"The fee of tx. default_value 0.0001 etp"
		);

        return options;
    }

    void set_defaults_from_config (po::variables_map& variables) override
    {
    }

    console_result invoke (std::ostream& output,
        std::ostream& cerr, bc::blockchain::block_chain_impl& blockchain) override;

    struct argument
    {
    	std::string from;
		std::string to;
		uint64_t amount;
		std::string message;
		uint64_t fee;
    } argument_;

    struct option
    {
    } option_;

};

}
} 
}
