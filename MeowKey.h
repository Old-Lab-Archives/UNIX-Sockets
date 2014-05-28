/* header for key management sockets */

/* Key management message header*/
struct sadb_msg /* SADB = Security Association DataBase*/
{
	u_int8_t sadb_msg_version; /*PF_KEY_V2*/
	u_int8_t sadb_msg_type;
	u_int8_t sadb_msg_errno; /*error indication*/
	u_int8_t sadb_msg_satype;
	u_int8_t sadb_msg_len; /*length of the header + extensions*/
	u_int8_t sadb_msg_reserve; /*zero on transmit...ignore on receive*/
	u_int8_t sadb_msg_seq; /*sequence number*/
	u_int8_t sadb_msg_pid; /* process ID of source or destination*/
};
/* Creating Static Security Association */
/* Security Association (SA) extension*/
struct sadb_sa
{
	u_int16_t sadb_sa_len; /*length of the extension*/
	u_int16_t sadb_sa_exttype; /*SADB_EXT_SA*/
	u_int32_t sadb_sa_spi; /*security parameters index(spi)*/
	u_int8_t sadb_sa_replay; /*replay window size or zero */
	u_int8_t sadb_sa_state; /* SA state */
	u_int8_t sadb_sa_auth; /*authentication algorithm*/
	u_int8_t sadb_sa_encrypt; /*encryption algorithm*/
	u_int32_t sadb_sa_flags; /* bit mask of flags */
};
/* address extension*/
struct sadb_address
{
u_int16_t sadb_address_len; /*length of extension + address */
u_int16_t sadb_address_exttype; /* SADB_EXT_ADDRESS*/
u_int8_t sadb_address_proto; /*IP protocol*/
u_int8_t sadb_address_prefixlen; /* significant bits in address */
u_int16_t sadb_address_reserved; /*reserved for extension*/
};
/* Key extension */
struct sadb_key
{
	u_int16_t sadb_key_len; /* length of the extension + key */
	u_int16_t sadb_key_exttype; /* SADB_EXT_KEY */
	u_int16_t sadb_key_bits; /*number of bits in key*/
	u_int16_t sadb_key_reserved; /*reserved for extension*/
};
/* Dynamically maintaining SAs */
struct sadb_supported
{
	u_int16_t sadb_supported_len; /* length of the extension + algorithm */
	u_int16_t sadb_supported_exttype; /* SADB_EXT_SUPPORTED */
	u_int32_t sadb_supported_reserved; /* reserved for future expansion */
};
struct sadb_alg
{
	u_int8_t sadb_alg_id; /* algorithm ID */
	u_int8_t sadb_alg_ivlen; /* IV length or zero*/
	u_int16_t sadb_alg_minbits; /*minimum key length*/
	u_int16_t sadb_alg_maxbits; /* maximum key length*/
	u_int16_t sadb_alg_reserved; /*reserved for future use*/
};
