/**
 * Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBBITCOIN_WALLET_PAYMENT_ADDRESS_HPP
#define LIBBITCOIN_WALLET_PAYMENT_ADDRESS_HPP

#include <algorithm>
#include <cstdint>
#include <string>
#include <bitcoin/bitcoin/compat.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/chain/script.hpp>
#include <bitcoin/bitcoin/math/checksum.hpp>
#include <bitcoin/bitcoin/math/elliptic_curve.hpp>
#include <bitcoin/bitcoin/math/hash.hpp>
#include <bitcoin/bitcoin/utility/data.hpp>
#include <bitcoin/bitcoin/wallet/ec_private.hpp>
#include <bitcoin/bitcoin/wallet/ec_public.hpp>

namespace libbitcoin {
namespace wallet {
    
static BC_CONSTEXPR size_t payment_size = 1u + short_hash_size + checksum_size;
typedef byte_array<payment_size> payment;

/// A class for working with non-stealth payment addresses.
class BC_API payment_address
{
public:
    static const uint8_t mainnet_p2kh;
    static const uint8_t mainnet_p2sh;

    /// Extract a payment address from an input or output script.
    /// The address will be invalid if and only if the script type is not
    /// supported or the script is itself invalid.
    static payment_address extract(const chain::script& script,
        uint8_t p2kh_version=mainnet_p2kh, uint8_t p2sh_version=mainnet_p2sh);

    /// Constructors.
    payment_address();
    payment_address(const payment& decoded);
    payment_address(const ec_private& secret);
    payment_address(const std::string& address);
    payment_address(const payment_address& other);
    payment_address(const short_hash& hash, uint8_t version=mainnet_p2kh);
    payment_address(const ec_public& point, uint8_t version=mainnet_p2kh);
    payment_address(const chain::script& script, uint8_t version=mainnet_p2sh);

    /// Operators.
    bool operator==(const payment_address& other) const;
    bool operator!=(const payment_address& other) const;
    payment_address& operator=(const payment_address& other);
    friend std::istream& operator>>(std::istream& in, payment_address& to);
    friend std::ostream& operator<<(std::ostream& out,
        const payment_address& of);

    /// Cast operators.
    operator const bool() const;
    operator const short_hash&() const;

    /// Serializer.
    std::string encoded() const;

    /// Accessors.
    uint8_t version() const;
    const short_hash& hash() const;

    /// Methods.
    payment to_payment() const;

private:
    /// Validators.
    static bool is_address(data_slice decoded);

    /// Factories.
    static payment_address from_string(const std::string& address);
    static payment_address from_payment(const payment& decoded);
    static payment_address from_private(const ec_private& secret);
    static payment_address from_public(const ec_public& point, uint8_t version);
    static payment_address from_script(const chain::script& script,
        uint8_t version);

    /// Members.
    /// These should be const, apart from the need to implement assignment.
    bool valid_;
    uint8_t version_;
    short_hash hash_;
};

} // namspace wallet
} // namspace libbitcoin

#endif
