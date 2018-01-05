#pragma once
#include <steem/plugins/json_rpc/utility.hpp>

#include <steem/chain/history_object.hpp>
#include <steem/chain/operation_notification.hpp>

#include <steem/protocol/types.hpp>

#include <fc/optional.hpp>
#include <fc/variant.hpp>
#include <fc/vector.hpp>

namespace steem { namespace plugins { namespace account_history {


namespace detail { class account_history_api_impl; }

struct api_operation_object
{
   api_operation_object() {}
   api_operation_object( const steem::chain::operation_object& op_obj ) :
      trx_id( op_obj.trx_id ),
      block( op_obj.block ),
      trx_in_block( op_obj.trx_in_block ),
      virtual_op( op_obj.virtual_op ),
      timestamp( op_obj.timestamp )
   {
      op = fc::raw::unpack_from_buffer< steem::protocol::operation >( op_obj.serialized_op );
   }

   steem::protocol::transaction_id_type trx_id;
   uint32_t                               block = 0;
   uint32_t                               trx_in_block = 0;
   uint16_t                               op_in_trx = 0;
   uint64_t                               virtual_op = 0;
   fc::time_point_sec                     timestamp;
   steem::protocol::operation             op;

   bool operator<( const api_operation_object& obj ) const
   {
      return std::tie( block, trx_in_block, op_in_trx, virtual_op ) < std::tie( obj.block, obj.trx_in_block, obj.op_in_trx, obj.virtual_op );
   }
};


struct get_ops_in_block_args
{
   uint32_t block_num;
   bool     only_virtual;
};

struct get_ops_in_block_return
{
   vector< api_operation_object > ops;
};


struct get_transaction_args
{
   steem::protocol::transaction_id_type id;
};

typedef steem::protocol::annotated_signed_transaction get_transaction_return;


struct get_account_history_args
{
   steem::protocol::account_name_type   account;
   uint64_t                               start = -1;
   uint32_t                               limit = 1000;
};

struct get_account_history_return
{
   std::map< uint32_t, api_operation_object > history;
};


class account_history_api
{
   public:
      account_history_api();
      ~account_history_api();

      DECLARE_API(
         (get_ops_in_block)
         (get_transaction)
         (get_account_history)
      )

   private:
      std::unique_ptr< detail::account_history_api_impl > my;
};

} } } // steem::plugins::account_history

FC_REFLECT( steem::plugins::account_history::api_operation_object,
   (trx_id)(block)(trx_in_block)(op_in_trx)(virtual_op)(timestamp)(op) )

FC_REFLECT( steem::plugins::account_history::get_ops_in_block_args,
   (block_num)(only_virtual) )

FC_REFLECT( steem::plugins::account_history::get_ops_in_block_return,
   (ops) )

FC_REFLECT( steem::plugins::account_history::get_transaction_args,
   (id) )

FC_REFLECT( steem::plugins::account_history::get_account_history_args,
   (account)(start)(limit) )

FC_REFLECT( steem::plugins::account_history::get_account_history_return,
   (history) )
