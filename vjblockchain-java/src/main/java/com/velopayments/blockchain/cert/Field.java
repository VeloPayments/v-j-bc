package com.velopayments.blockchain.cert;

public final class Field {
    /* Begin of the pre-defined short field codes range. */
    public static final int VELO_RESERVED_BEGIN = 0x0000;

    /* Reserved Zero-Tag. */
    public static final int RESERVED_ZERO_TAG = 0x0000;
    /* Certificate Format Version. Unsigned 32-bit Big-Endian. */
    public static final int CERTIFICATE_VERSION = 0x0001;
    /* Reserved fields */
    public static final int VELO_RESERVED_0002 = 0x0002;
    public static final int VELO_RESERVED_0003 = 0x0003;
    public static final int VELO_RESERVED_0004 = 0x0004;
    public static final int VELO_RESERVED_0005 = 0x0005;
    public static final int VELO_RESERVED_0006 = 0x0006;
    public static final int VELO_RESERVED_0007 = 0x0007;
    public static final int VELO_RESERVED_0008 = 0x0008;
    public static final int VELO_RESERVED_0009 = 0x0009;
    public static final int VELO_RESERVED_000A = 0x000A;
    public static final int VELO_RESERVED_000B = 0x000B;
    public static final int VELO_RESERVED_000C = 0x000C;
    public static final int VELO_RESERVED_000D = 0x000D;
    public static final int VELO_RESERVED_000E = 0x000E;
    public static final int VELO_RESERVED_000F = 0x000F;

    /* Certificate valid from date (uint64_t seconds from Jan-01-1970) */
    public static final int CERTIFICATE_VALID_FROM = 0x0010;
    /* Certificate valid to date (uint64_t seconds from Jan-01-1970) */
    public static final int CERTIFICATE_VALID_TO = 0x0011;

    /* reserved fields */
    public static final int VELO_RESERVED_0012 = 0x0012;
    public static final int VELO_RESERVED_0013 = 0x0013;
    public static final int VELO_RESERVED_0014 = 0x0014;
    public static final int VELO_RESERVED_0015 = 0x0015;
    public static final int VELO_RESERVED_0016 = 0x0016;
    public static final int VELO_RESERVED_0017 = 0x0017;
    public static final int VELO_RESERVED_0018 = 0x0018;
    public static final int VELO_RESERVED_0019 = 0x0019;
    public static final int VELO_RESERVED_001A = 0x001A;
    public static final int VELO_RESERVED_001B = 0x001B;
    public static final int VELO_RESERVED_001C = 0x001C;
    public static final int VELO_RESERVED_001D = 0x001D;
    public static final int VELO_RESERVED_001E = 0x001E;
    public static final int VELO_RESERVED_001F = 0x001F;

    /* Certificate Crypto Suite. (uint16_t identifier) */
    public static final int CERTIFICATE_CRYPTO_SUITE = 0x0020;
    /* reserved fields */
    public static final int VELO_RESERVED_0021 = 0x0021;
    public static final int VELO_RESERVED_0022 = 0x0022;
    public static final int VELO_RESERVED_0023 = 0x0023;
    public static final int VELO_RESERVED_0024 = 0x0024;
    public static final int VELO_RESERVED_0025 = 0x0025;
    public static final int VELO_RESERVED_0026 = 0x0026;
    public static final int VELO_RESERVED_0027 = 0x0027;
    public static final int VELO_RESERVED_0028 = 0x0028;
    public static final int VELO_RESERVED_0029 = 0x0029;
    public static final int VELO_RESERVED_002A = 0x002A;
    public static final int VELO_RESERVED_002B = 0x002B;
    public static final int VELO_RESERVED_002C = 0x002C;
    public static final int VELO_RESERVED_002D = 0x002D;
    public static final int VELO_RESERVED_002E = 0x002E;
    public static final int VELO_RESERVED_002F = 0x002F;

    /* Certificate Type. (128-bit UUID) */
    public static final int CERTIFICATE_TYPE = 0x0030;
    /* reserved fields */
    public static final int VELO_RESERVED_0031 = 0x0031;
    public static final int VELO_RESERVED_0032 = 0x0032;
    public static final int VELO_RESERVED_0033 = 0x0033;
    public static final int VELO_RESERVED_0034 = 0x0034;
    public static final int VELO_RESERVED_0035 = 0x0035;
    public static final int VELO_RESERVED_0036 = 0x0036;
    public static final int VELO_RESERVED_0037 = 0x0037;
    /* Certificate ID.  UUID. */
    public static final int CERTIFICATE_ID = 0x0038;
    /* Previous Certificate ID.  UUID. */
    public static final int PREVIOUS_CERTIFICATE_ID = 0x0039;
    /* Next Certificate ID.  UUID. */
    public static final int NEXT_CERTIFICATE_ID = 0x003A;
    /* reserved fields */
    public static final int VELO_RESERVED_003B = 0x003B;
    public static final int VELO_RESERVED_003C = 0x003C;
    public static final int VELO_RESERVED_003D = 0x003D;
    public static final int VELO_RESERVED_003E = 0x003E;
    public static final int VELO_RESERVED_003F = 0x003F;

    /* Artifact Type. (128-bit UUID) */
    public static final int ARTIFACT_TYPE = 0x0040;
    /* Artifact ID. (128-bit UUID) */
    public static final int ARTIFACT_ID = 0x0041;

    public static final int PREVIOUS_ARTIFACT_STATE = 0x0042;
    public static final int NEW_ARTIFACT_STATE = 0x0043;
    /* reserved fields */
    public static final int VELO_RESERVED_0044 = 0x0044;
    public static final int VELO_RESERVED_0045 = 0x0045;
    public static final int VELO_RESERVED_0046 = 0x0046;
    public static final int VELO_RESERVED_0047 = 0x0047;
    public static final int VELO_RESERVED_0048 = 0x0048;
    public static final int VELO_RESERVED_0049 = 0x0049;
    public static final int VELO_RESERVED_004A = 0x004A;
    public static final int VELO_RESERVED_004B = 0x004B;
    public static final int VELO_RESERVED_004C = 0x004C;
    public static final int VELO_RESERVED_004D = 0x004D;
    public static final int VELO_RESERVED_004E = 0x004E;
    public static final int VELO_RESERVED_004F = 0x004F;

    /* Signer ID. (128-bit UUID) */
    public static final int SIGNER_ID = 0x0050;
    /* Signature. (Crypto Suite Dependent Length and Content) */
    public static final int SIGNATURE = 0x0051;
    /* Public Encryption Key (Crypto Suite Dependent Length and Content) */
    public static final int PUBLIC_ENCRYPTION_KEY = 0x0052;
    /* Public Signing Key (Crypto Suite Dependent Length and Content) */
    public static final int PUBLIC_SIGNING_KEY = 0x0053;
    /* Private Encryption Key (Crypto Suite Dependent Length and Content) */
    public static final int PRIVATE_ENCRYPTION_KEY = 0x0054;
    /* Private Signing Key (Crypto Suite Dependent Length and Content) */
    public static final int PRIVATE_SIGNING_KEY = 0x0055;
    /* reserved fields */
    public static final int VELO_RESERVED_0056 = 0x0056;
    public static final int VELO_RESERVED_0057 = 0x0057;
    public static final int VELO_RESERVED_0058 = 0x0058;
    public static final int VELO_RESERVED_0059 = 0x0059;
    public static final int VELO_RESERVED_005A = 0x005A;
    public static final int VELO_RESERVED_005B = 0x005B;
    public static final int VELO_RESERVED_005C = 0x005C;
    public static final int VELO_RESERVED_005D = 0x005D;
    public static final int VELO_RESERVED_005E = 0x005E;
    public static final int VELO_RESERVED_005F = 0x005F;

    /* Grant Descriptor Tuple */
    public static final int GRANT_DESCRIPTOR_TUPLE = 0x0060;
    /* Grant Description (UTF-8 characters excluding null terminator) */
    public static final int GRANT_DESCRIPTION = 0x0061;
    /* The expected type of the subject entity. */
    public static final int GRANT_SUBJECT_TYPE = 0x0062;
    /* The expected type of the object artifact. */
    public static final int GRANT_OBJECT_TYPE = 0x0063;
    /* The explicit transaction type for this grant. */
    public static final int GRANT_TRANSACTION_TYPE = 0x0064;
    /* A tuple value of grant ID, subject UUID, and object UUID. */
    public static final int GRANT_TUPLE = 0x0065;
    /* UUID of the subject of a grant. */
    public static final int GRANT_SUBJECT = 0x0066;
    /* UUID of the object of a grant. */
    public static final int GRANT_OBJECT = 0x0067;
    /* UUID of the first auxiliary type of a grant descriptor. */
    public static final int GRANT_AUXILIARY_TYPE_1 = 0x0068;
    /* UUID of the second auxiliary type of a grant descriptor. */
    public static final int GRANT_AUXILIARY_TYPE_2 = 0x0069;
    /* UUID of the third auxiliary type of a grant descriptor. */
    public static final int GRANT_AUXILIARY_TYPE_3 = 0x006A;
    /* UUID of the fourth auxiliary type of a grant descriptor. */
    public static final int GRANT_AUXILIARY_TYPE_4 = 0x006B;
    /* UUID of the first auxiliary of a grant. */
    public static final int GRANT_AUXILIARY_1 = 0x006C;
    /* UUID of the second auxiliary of a grant. */
    public static final int GRANT_AUXILIARY_2 = 0x006D;
    /* UUID of the third auxiliary of a grant. */
    public static final int GRANT_AUXILIARY_3 = 0x006E;
    /* UUID of the fourth auxiliary of a grant. */
    public static final int GRANT_AUXILIARY_4 = 0x006F;

    /* Artifact Type State Transition Tuple. */
    public static final int ARTIFACT_TYPE_STATE_TRANSITION_TUPLE = 0x0070;
    /* User Field Mapping Tuple. */
    public static final int USER_FIELD_MAPPING_TUPLE = 0x0071;
    /* Field Mapping Tuple. */
    public static final int FIELD_MAPPING_TUPLE = 0x0072;
    /* Contract Bytecode. */
    public static final int CONTRACT_BYTECODE = 0x0073;
    /* Short Field Type. Unsigned 16-bit Big Endian. */
    public static final int SHORT_FIELD_TYPE = 0x0074;
    /* Long Field Type. UUID. */
    public static final int LONG_FIELD_TYPE = 0x0075;
    /* Transaction Type UUID. */
    public static final int TRANSACTION_TYPE = 0x0076;
    /* reserved fields */
    public static final int VELO_RESERVED_0077 = 0x0077;
    public static final int VELO_RESERVED_0078 = 0x0078;
    public static final int VELO_RESERVED_0079 = 0x0079;
    public static final int VELO_RESERVED_007A = 0x007A;
    public static final int VELO_RESERVED_007B = 0x007B;
    public static final int VELO_RESERVED_007C = 0x007C;
    public static final int VELO_RESERVED_007D = 0x007D;
    public static final int VELO_RESERVED_007E = 0x007E;
    public static final int VELO_RESERVED_007F = 0x007F;

    /* Block UUID. */
    public static final int BLOCK_UUID = 0x0080;
    /* Previous Block UUID. */
    public static final int PREVIOUS_BLOCK_UUID = 0x0081;
    /* Previous Block Hash. */
    public static final int PREVIOUS_BLOCK_HASH = 0x0082;
    /* Block Height. */
    public static final int BLOCK_HEIGHT = 0x0083;
    /* Wrapped Transaction Tuple. */
    public static final int WRAPPED_TRANSACTION_TUPLE = 0x0084;
    /* Block Signature Tuple. */
    public static final int BLOCK_SIGNATURE_TUPLE = 0x0085;

    /* reserved fields */
    public static final int VELO_RESERVED_0086 = 0x0086;
    public static final int VELO_RESERVED_0087 = 0x0087;
    public static final int VELO_RESERVED_0088 = 0x0088;
    public static final int VELO_RESERVED_0089 = 0x0089;
    public static final int VELO_RESERVED_008A = 0x008A;
    public static final int VELO_RESERVED_008B = 0x008B;
    public static final int VELO_RESERVED_008C = 0x008C;
    public static final int VELO_RESERVED_008D = 0x008D;
    public static final int VELO_RESERVED_008E = 0x008E;
    public static final int VELO_RESERVED_008F = 0x008F;

    /* Vote consensus message. */
    public static final int VCCERT_FIELD_TYPE_AGENT_SUBTYPE_VOTE = 0x0090;
    /* Commit consensus message. */
    public static final int VCCERT_FIELD_TYPE_AGENT_SUBTYPE_COMMIT = 0x0091;
    /* Proposal consensus message. */
    public static final int VCCERT_FIELD_TYPE_AGENT_SUBTYPE_PROPOSAL = 0x0092;
    /* Recovery consensus message. */
    public static final int VCCERT_FIELD_TYPE_AGENT_SUBTYPE_RECOVERY = 0x0093;
    /* Heartbeat consensus message. */
    public static final int VCCERT_FIELD_TYPE_AGENT_SUBTYPE_HEARTBEAT = 0x0094;

    /* reserved fields */
    public static final int VELO_RESERVED_0095 = 0x0095;
    public static final int VELO_RESERVED_0096 = 0x0096;
    public static final int VELO_RESERVED_0097 = 0x0097;
    public static final int VELO_RESERVED_0098 = 0x0098;
    public static final int VELO_RESERVED_0099 = 0x0099;
    public static final int VELO_RESERVED_009A = 0x009A;
    public static final int VELO_RESERVED_009B = 0x009B;
    public static final int VELO_RESERVED_009C = 0x009C;
    public static final int VELO_RESERVED_009D = 0x009D;
    public static final int VELO_RESERVED_009E = 0x009E;
    public static final int VELO_RESERVED_009F = 0x009F;

    /* Encrypted Shared Secret Fragment. */
    public static final int VELO_ENCRYPTED_SHARED_SECRET_FRAGMENT = 0x00A0;
    /* Entity UUID for this encrypted shared secret. */
    public static final int VELO_ENCRYPTED_SHARED_SECRET_ENTITY_UUID = 0x00A1;
    /* Encrypted shared secret key. */
    public static final int VELO_ENCRYPTED_SHARED_SECRET_KEY = 0x00A2;

    /* reserved fields */
    public static final int VELO_RESERVED_00A3 = 0x00A3;
    public static final int VELO_RESERVED_00A4 = 0x00A4;
    public static final int VELO_RESERVED_00A5 = 0x00A5;
    public static final int VELO_RESERVED_00A6 = 0x00A6;
    public static final int VELO_RESERVED_00A7 = 0x00A7;
    public static final int VELO_RESERVED_00A8 = 0x00A8;
    public static final int VELO_RESERVED_00A9 = 0x00A9;
    public static final int VELO_RESERVED_00AA = 0x00AA;
    public static final int VELO_RESERVED_00AB = 0x00AB;
    public static final int VELO_RESERVED_00AC = 0x00AC;
    public static final int VELO_RESERVED_00AD = 0x00AD;
    public static final int VELO_RESERVED_00AE = 0x00AE;
    public static final int VELO_RESERVED_00AF = 0x00AF;

    public static final int VELO_RESERVED_00B0 = 0x00B0;
    public static final int VELO_RESERVED_00B1 = 0x00B1;
    public static final int VELO_RESERVED_00B2 = 0x00B2;
    public static final int VELO_RESERVED_00B3 = 0x00B3;
    public static final int VELO_RESERVED_00B4 = 0x00B4;
    public static final int VELO_RESERVED_00B5 = 0x00B5;
    public static final int VELO_RESERVED_00B6 = 0x00B6;
    public static final int VELO_RESERVED_00B7 = 0x00B7;
    public static final int VELO_RESERVED_00B8 = 0x00B8;
    public static final int VELO_RESERVED_00B9 = 0x00B9;
    public static final int VELO_RESERVED_00BA = 0x00BA;
    public static final int VELO_RESERVED_00BB = 0x00BB;
    public static final int VELO_RESERVED_00BC = 0x00BC;
    public static final int VELO_RESERVED_00BD = 0x00BD;
    public static final int VELO_RESERVED_00BE = 0x00BE;
    public static final int VELO_RESERVED_00BF = 0x00BF;

    public static final int VELO_RESERVED_00C0 = 0x00C0;
    public static final int VELO_RESERVED_00C1 = 0x00C1;
    public static final int VELO_RESERVED_00C2 = 0x00C2;
    public static final int VELO_RESERVED_00C3 = 0x00C3;
    public static final int VELO_RESERVED_00C4 = 0x00C4;
    public static final int VELO_RESERVED_00C5 = 0x00C5;
    public static final int VELO_RESERVED_00C6 = 0x00C6;
    public static final int VELO_RESERVED_00C7 = 0x00C7;
    public static final int VELO_RESERVED_00C8 = 0x00C8;
    public static final int VELO_RESERVED_00C9 = 0x00C9;
    public static final int VELO_RESERVED_00CA = 0x00CA;
    public static final int VELO_RESERVED_00CB = 0x00CB;
    public static final int VELO_RESERVED_00CC = 0x00CC;
    public static final int VELO_RESERVED_00CD = 0x00CD;
    public static final int VELO_RESERVED_00CE = 0x00CE;
    public static final int VELO_RESERVED_00CF = 0x00CF;

    public static final int VELO_RESERVED_00D0 = 0x00D0;
    public static final int VELO_RESERVED_00D1 = 0x00D1;
    public static final int VELO_RESERVED_00D2 = 0x00D2;
    public static final int VELO_RESERVED_00D3 = 0x00D3;
    public static final int VELO_RESERVED_00D4 = 0x00D4;
    public static final int VELO_RESERVED_00D5 = 0x00D5;
    public static final int VELO_RESERVED_00D6 = 0x00D6;
    public static final int VELO_RESERVED_00D7 = 0x00D7;
    public static final int VELO_RESERVED_00D8 = 0x00D8;
    public static final int VELO_RESERVED_00D9 = 0x00D9;
    public static final int VELO_RESERVED_00DA = 0x00DA;
    public static final int VELO_RESERVED_00DB = 0x00DB;
    public static final int VELO_RESERVED_00DC = 0x00DC;
    public static final int VELO_RESERVED_00DD = 0x00DD;
    public static final int VELO_RESERVED_00DE = 0x00DE;
    public static final int VELO_RESERVED_00DF = 0x00DF;

    public static final int VELO_RESERVED_00E0 = 0x00E0;
    public static final int VELO_RESERVED_00E1 = 0x00E1;
    public static final int VELO_RESERVED_00E2 = 0x00E2;
    public static final int VELO_RESERVED_00E3 = 0x00E3;
    public static final int VELO_RESERVED_00E4 = 0x00E4;
    public static final int VELO_RESERVED_00E5 = 0x00E5;
    public static final int VELO_RESERVED_00E6 = 0x00E6;
    public static final int VELO_RESERVED_00E7 = 0x00E7;
    public static final int VELO_RESERVED_00E8 = 0x00E8;
    public static final int VELO_RESERVED_00E9 = 0x00E9;
    public static final int VELO_RESERVED_00EA = 0x00EA;
    public static final int VELO_RESERVED_00EB = 0x00EB;
    public static final int VELO_RESERVED_00EC = 0x00EC;
    public static final int VELO_RESERVED_00ED = 0x00ED;
    public static final int VELO_RESERVED_00EE = 0x00EE;
    public static final int VELO_RESERVED_00EF = 0x00EF;

    public static final int VELO_RESERVED_00F0 = 0x00F0;
    public static final int VELO_RESERVED_00F1 = 0x00F1;
    public static final int VELO_RESERVED_00F2 = 0x00F2;
    public static final int VELO_RESERVED_00F3 = 0x00F3;
    public static final int VELO_RESERVED_00F4 = 0x00F4;
    public static final int VELO_RESERVED_00F5 = 0x00F5;
    public static final int VELO_RESERVED_00F6 = 0x00F6;
    public static final int VELO_RESERVED_00F7 = 0x00F7;
    public static final int VELO_RESERVED_00F8 = 0x00F8;
    public static final int VELO_RESERVED_00F9 = 0x00F9;
    public static final int VELO_RESERVED_00FA = 0x00FA;
    public static final int VELO_RESERVED_00FB = 0x00FB;
    public static final int VELO_RESERVED_00FC = 0x00FC;
    public static final int VELO_RESERVED_00FD = 0x00FD;
    public static final int VELO_RESERVED_00FE = 0x00FE;
    public static final int VELO_RESERVED_00FF = 0x00FF;

    public static final int VELO_RESERVED_0100 = 0x0100;
    public static final int VELO_RESERVED_0101 = 0x0101;
    public static final int VELO_RESERVED_0102 = 0x0102;
    public static final int VELO_RESERVED_0103 = 0x0103;
    public static final int VELO_RESERVED_0104 = 0x0104;
    public static final int VELO_RESERVED_0105 = 0x0105;
    public static final int VELO_RESERVED_0106 = 0x0106;
    public static final int VELO_RESERVED_0107 = 0x0107;
    public static final int VELO_RESERVED_0108 = 0x0108;
    public static final int VELO_RESERVED_0109 = 0x0109;
    public static final int VELO_RESERVED_010A = 0x010A;
    public static final int VELO_RESERVED_010B = 0x010B;
    public static final int VELO_RESERVED_010C = 0x010C;
    public static final int VELO_RESERVED_010D = 0x010D;
    public static final int VELO_RESERVED_010E = 0x010E;
    public static final int VELO_RESERVED_010F = 0x010F;

    public static final int VELO_RESERVED_0110 = 0x0110;
    public static final int VELO_RESERVED_0111 = 0x0111;
    public static final int VELO_RESERVED_0112 = 0x0112;
    public static final int VELO_RESERVED_0113 = 0x0113;
    public static final int VELO_RESERVED_0114 = 0x0114;
    public static final int VELO_RESERVED_0115 = 0x0115;
    public static final int VELO_RESERVED_0116 = 0x0116;
    public static final int VELO_RESERVED_0117 = 0x0117;
    public static final int VELO_RESERVED_0118 = 0x0118;
    public static final int VELO_RESERVED_0119 = 0x0119;
    public static final int VELO_RESERVED_011A = 0x011A;
    public static final int VELO_RESERVED_011B = 0x011B;
    public static final int VELO_RESERVED_011C = 0x011C;
    public static final int VELO_RESERVED_011D = 0x011D;
    public static final int VELO_RESERVED_011E = 0x011E;
    public static final int VELO_RESERVED_011F = 0x011F;

    public static final int VELO_RESERVED_0120 = 0x0120;
    public static final int VELO_RESERVED_0121 = 0x0121;
    public static final int VELO_RESERVED_0122 = 0x0122;
    public static final int VELO_RESERVED_0123 = 0x0123;
    public static final int VELO_RESERVED_0124 = 0x0124;
    public static final int VELO_RESERVED_0125 = 0x0125;
    public static final int VELO_RESERVED_0126 = 0x0126;
    public static final int VELO_RESERVED_0127 = 0x0127;
    public static final int VELO_RESERVED_0128 = 0x0128;
    public static final int VELO_RESERVED_0129 = 0x0129;
    public static final int VELO_RESERVED_012A = 0x012A;
    public static final int VELO_RESERVED_012B = 0x012B;
    public static final int VELO_RESERVED_012C = 0x012C;
    public static final int VELO_RESERVED_012D = 0x012D;
    public static final int VELO_RESERVED_012E = 0x012E;
    public static final int VELO_RESERVED_012F = 0x012F;

    public static final int VELO_RESERVED_0130 = 0x0130;
    public static final int VELO_RESERVED_0131 = 0x0131;
    public static final int VELO_RESERVED_0132 = 0x0132;
    public static final int VELO_RESERVED_0133 = 0x0133;
    public static final int VELO_RESERVED_0134 = 0x0134;
    public static final int VELO_RESERVED_0135 = 0x0135;
    public static final int VELO_RESERVED_0136 = 0x0136;
    public static final int VELO_RESERVED_0137 = 0x0137;
    public static final int VELO_RESERVED_0138 = 0x0138;
    public static final int VELO_RESERVED_0139 = 0x0139;
    public static final int VELO_RESERVED_013A = 0x013A;
    public static final int VELO_RESERVED_013B = 0x013B;
    public static final int VELO_RESERVED_013C = 0x013C;
    public static final int VELO_RESERVED_013D = 0x013D;
    public static final int VELO_RESERVED_013E = 0x013E;
    public static final int VELO_RESERVED_013F = 0x013F;

    public static final int VELO_RESERVED_0140 = 0x0140;
    public static final int VELO_RESERVED_0141 = 0x0141;
    public static final int VELO_RESERVED_0142 = 0x0142;
    public static final int VELO_RESERVED_0143 = 0x0143;
    public static final int VELO_RESERVED_0144 = 0x0144;
    public static final int VELO_RESERVED_0145 = 0x0145;
    public static final int VELO_RESERVED_0146 = 0x0146;
    public static final int VELO_RESERVED_0147 = 0x0147;
    public static final int VELO_RESERVED_0148 = 0x0148;
    public static final int VELO_RESERVED_0149 = 0x0149;
    public static final int VELO_RESERVED_014A = 0x014A;
    public static final int VELO_RESERVED_014B = 0x014B;
    public static final int VELO_RESERVED_014C = 0x014C;
    public static final int VELO_RESERVED_014D = 0x014D;
    public static final int VELO_RESERVED_014E = 0x014E;
    public static final int VELO_RESERVED_014F = 0x014F;

    public static final int VELO_RESERVED_0150 = 0x0150;
    public static final int VELO_RESERVED_0151 = 0x0151;
    public static final int VELO_RESERVED_0152 = 0x0152;
    public static final int VELO_RESERVED_0153 = 0x0153;
    public static final int VELO_RESERVED_0154 = 0x0154;
    public static final int VELO_RESERVED_0155 = 0x0155;
    public static final int VELO_RESERVED_0156 = 0x0156;
    public static final int VELO_RESERVED_0157 = 0x0157;
    public static final int VELO_RESERVED_0158 = 0x0158;
    public static final int VELO_RESERVED_0159 = 0x0159;
    public static final int VELO_RESERVED_015A = 0x015A;
    public static final int VELO_RESERVED_015B = 0x015B;
    public static final int VELO_RESERVED_015C = 0x015C;
    public static final int VELO_RESERVED_015D = 0x015D;
    public static final int VELO_RESERVED_015E = 0x015E;
    public static final int VELO_RESERVED_015F = 0x015F;

    public static final int VELO_RESERVED_0160 = 0x0160;
    public static final int VELO_RESERVED_0161 = 0x0161;
    public static final int VELO_RESERVED_0162 = 0x0162;
    public static final int VELO_RESERVED_0163 = 0x0163;
    public static final int VELO_RESERVED_0164 = 0x0164;
    public static final int VELO_RESERVED_0165 = 0x0165;
    public static final int VELO_RESERVED_0166 = 0x0166;
    public static final int VELO_RESERVED_0167 = 0x0167;
    public static final int VELO_RESERVED_0168 = 0x0168;
    public static final int VELO_RESERVED_0169 = 0x0169;
    public static final int VELO_RESERVED_016A = 0x016A;
    public static final int VELO_RESERVED_016B = 0x016B;
    public static final int VELO_RESERVED_016C = 0x016C;
    public static final int VELO_RESERVED_016D = 0x016D;
    public static final int VELO_RESERVED_016E = 0x016E;
    public static final int VELO_RESERVED_016F = 0x016F;

    public static final int VELO_RESERVED_0170 = 0x0170;
    public static final int VELO_RESERVED_0171 = 0x0171;
    public static final int VELO_RESERVED_0172 = 0x0172;
    public static final int VELO_RESERVED_0173 = 0x0173;
    public static final int VELO_RESERVED_0174 = 0x0174;
    public static final int VELO_RESERVED_0175 = 0x0175;
    public static final int VELO_RESERVED_0176 = 0x0176;
    public static final int VELO_RESERVED_0177 = 0x0177;
    public static final int VELO_RESERVED_0178 = 0x0178;
    public static final int VELO_RESERVED_0179 = 0x0179;
    public static final int VELO_RESERVED_017A = 0x017A;
    public static final int VELO_RESERVED_017B = 0x017B;
    public static final int VELO_RESERVED_017C = 0x017C;
    public static final int VELO_RESERVED_017D = 0x017D;
    public static final int VELO_RESERVED_017E = 0x017E;
    public static final int VELO_RESERVED_017F = 0x017F;

    public static final int VELO_RESERVED_0180 = 0x0180;
    public static final int VELO_RESERVED_0181 = 0x0181;
    public static final int VELO_RESERVED_0182 = 0x0182;
    public static final int VELO_RESERVED_0183 = 0x0183;
    public static final int VELO_RESERVED_0184 = 0x0184;
    public static final int VELO_RESERVED_0185 = 0x0185;
    public static final int VELO_RESERVED_0186 = 0x0186;
    public static final int VELO_RESERVED_0187 = 0x0187;
    public static final int VELO_RESERVED_0188 = 0x0188;
    public static final int VELO_RESERVED_0189 = 0x0189;
    public static final int VELO_RESERVED_018A = 0x018A;
    public static final int VELO_RESERVED_018B = 0x018B;
    public static final int VELO_RESERVED_018C = 0x018C;
    public static final int VELO_RESERVED_018D = 0x018D;
    public static final int VELO_RESERVED_018E = 0x018E;
    public static final int VELO_RESERVED_018F = 0x018F;

    public static final int VELO_RESERVED_0190 = 0x0190;
    public static final int VELO_RESERVED_0191 = 0x0191;
    public static final int VELO_RESERVED_0192 = 0x0192;
    public static final int VELO_RESERVED_0193 = 0x0193;
    public static final int VELO_RESERVED_0194 = 0x0194;
    public static final int VELO_RESERVED_0195 = 0x0195;
    public static final int VELO_RESERVED_0196 = 0x0196;
    public static final int VELO_RESERVED_0197 = 0x0197;
    public static final int VELO_RESERVED_0198 = 0x0198;
    public static final int VELO_RESERVED_0199 = 0x0199;
    public static final int VELO_RESERVED_019A = 0x019A;
    public static final int VELO_RESERVED_019B = 0x019B;
    public static final int VELO_RESERVED_019C = 0x019C;
    public static final int VELO_RESERVED_019D = 0x019D;
    public static final int VELO_RESERVED_019E = 0x019E;
    public static final int VELO_RESERVED_019F = 0x019F;

    public static final int VELO_RESERVED_01A0 = 0x01A0;
    public static final int VELO_RESERVED_01A1 = 0x01A1;
    public static final int VELO_RESERVED_01A2 = 0x01A2;
    public static final int VELO_RESERVED_01A3 = 0x01A3;
    public static final int VELO_RESERVED_01A4 = 0x01A4;
    public static final int VELO_RESERVED_01A5 = 0x01A5;
    public static final int VELO_RESERVED_01A6 = 0x01A6;
    public static final int VELO_RESERVED_01A7 = 0x01A7;
    public static final int VELO_RESERVED_01A8 = 0x01A8;
    public static final int VELO_RESERVED_01A9 = 0x01A9;
    public static final int VELO_RESERVED_01AA = 0x01AA;
    public static final int VELO_RESERVED_01AB = 0x01AB;
    public static final int VELO_RESERVED_01AC = 0x01AC;
    public static final int VELO_RESERVED_01AD = 0x01AD;
    public static final int VELO_RESERVED_01AE = 0x01AE;
    public static final int VELO_RESERVED_01AF = 0x01AF;

    public static final int VELO_RESERVED_01B0 = 0x01B0;
    public static final int VELO_RESERVED_01B1 = 0x01B1;
    public static final int VELO_RESERVED_01B2 = 0x01B2;
    public static final int VELO_RESERVED_01B3 = 0x01B3;
    public static final int VELO_RESERVED_01B4 = 0x01B4;
    public static final int VELO_RESERVED_01B5 = 0x01B5;
    public static final int VELO_RESERVED_01B6 = 0x01B6;
    public static final int VELO_RESERVED_01B7 = 0x01B7;
    public static final int VELO_RESERVED_01B8 = 0x01B8;
    public static final int VELO_RESERVED_01B9 = 0x01B9;
    public static final int VELO_RESERVED_01BA = 0x01BA;
    public static final int VELO_RESERVED_01BB = 0x01BB;
    public static final int VELO_RESERVED_01BC = 0x01BC;
    public static final int VELO_RESERVED_01BD = 0x01BD;
    public static final int VELO_RESERVED_01BE = 0x01BE;
    public static final int VELO_RESERVED_01BF = 0x01BF;

    public static final int VELO_RESERVED_01C0 = 0x01C0;
    public static final int VELO_RESERVED_01C1 = 0x01C1;
    public static final int VELO_RESERVED_01C2 = 0x01C2;
    public static final int VELO_RESERVED_01C3 = 0x01C3;
    public static final int VELO_RESERVED_01C4 = 0x01C4;
    public static final int VELO_RESERVED_01C5 = 0x01C5;
    public static final int VELO_RESERVED_01C6 = 0x01C6;
    public static final int VELO_RESERVED_01C7 = 0x01C7;
    public static final int VELO_RESERVED_01C8 = 0x01C8;
    public static final int VELO_RESERVED_01C9 = 0x01C9;
    public static final int VELO_RESERVED_01CA = 0x01CA;
    public static final int VELO_RESERVED_01CB = 0x01CB;
    public static final int VELO_RESERVED_01CC = 0x01CC;
    public static final int VELO_RESERVED_01CD = 0x01CD;
    public static final int VELO_RESERVED_01CE = 0x01CE;
    public static final int VELO_RESERVED_01CF = 0x01CF;

    public static final int VELO_RESERVED_01D0 = 0x01D0;
    public static final int VELO_RESERVED_01D1 = 0x01D1;
    public static final int VELO_RESERVED_01D2 = 0x01D2;
    public static final int VELO_RESERVED_01D3 = 0x01D3;
    public static final int VELO_RESERVED_01D4 = 0x01D4;
    public static final int VELO_RESERVED_01D5 = 0x01D5;
    public static final int VELO_RESERVED_01D6 = 0x01D6;
    public static final int VELO_RESERVED_01D7 = 0x01D7;
    public static final int VELO_RESERVED_01D8 = 0x01D8;
    public static final int VELO_RESERVED_01D9 = 0x01D9;
    public static final int VELO_RESERVED_01DA = 0x01DA;
    public static final int VELO_RESERVED_01DB = 0x01DB;
    public static final int VELO_RESERVED_01DC = 0x01DC;
    public static final int VELO_RESERVED_01DD = 0x01DD;
    public static final int VELO_RESERVED_01DE = 0x01DE;
    public static final int VELO_RESERVED_01DF = 0x01DF;

    public static final int VELO_RESERVED_01E0 = 0x01E0;
    public static final int VELO_RESERVED_01E1 = 0x01E1;
    public static final int VELO_RESERVED_01E2 = 0x01E2;
    public static final int VELO_RESERVED_01E3 = 0x01E3;
    public static final int VELO_RESERVED_01E4 = 0x01E4;
    public static final int VELO_RESERVED_01E5 = 0x01E5;
    public static final int VELO_RESERVED_01E6 = 0x01E6;
    public static final int VELO_RESERVED_01E7 = 0x01E7;
    public static final int VELO_RESERVED_01E8 = 0x01E8;
    public static final int VELO_RESERVED_01E9 = 0x01E9;
    public static final int VELO_RESERVED_01EA = 0x01EA;
    public static final int VELO_RESERVED_01EB = 0x01EB;
    public static final int VELO_RESERVED_01EC = 0x01EC;
    public static final int VELO_RESERVED_01ED = 0x01ED;
    public static final int VELO_RESERVED_01EE = 0x01EE;
    public static final int VELO_RESERVED_01EF = 0x01EF;

    public static final int VELO_RESERVED_01F0 = 0x01F0;
    public static final int VELO_RESERVED_01F1 = 0x01F1;
    public static final int VELO_RESERVED_01F2 = 0x01F2;
    public static final int VELO_RESERVED_01F3 = 0x01F3;
    public static final int VELO_RESERVED_01F4 = 0x01F4;
    public static final int VELO_RESERVED_01F5 = 0x01F5;
    public static final int VELO_RESERVED_01F6 = 0x01F6;
    public static final int VELO_RESERVED_01F7 = 0x01F7;
    public static final int VELO_RESERVED_01F8 = 0x01F8;
    public static final int VELO_RESERVED_01F9 = 0x01F9;
    public static final int VELO_RESERVED_01FA = 0x01FA;
    public static final int VELO_RESERVED_01FB = 0x01FB;
    public static final int VELO_RESERVED_01FC = 0x01FC;
    public static final int VELO_RESERVED_01FD = 0x01FD;
    public static final int VELO_RESERVED_01FE = 0x01FE;
    public static final int VELO_RESERVED_01FF = 0x01FF;

    public static final int VELO_RESERVED_0200 = 0x0200;
    public static final int VELO_RESERVED_0201 = 0x0201;
    public static final int VELO_RESERVED_0202 = 0x0202;
    public static final int VELO_RESERVED_0203 = 0x0203;
    public static final int VELO_RESERVED_0204 = 0x0204;
    public static final int VELO_RESERVED_0205 = 0x0205;
    public static final int VELO_RESERVED_0206 = 0x0206;
    public static final int VELO_RESERVED_0207 = 0x0207;
    public static final int VELO_RESERVED_0208 = 0x0208;
    public static final int VELO_RESERVED_0209 = 0x0209;
    public static final int VELO_RESERVED_020A = 0x020A;
    public static final int VELO_RESERVED_020B = 0x020B;
    public static final int VELO_RESERVED_020C = 0x020C;
    public static final int VELO_RESERVED_020D = 0x020D;
    public static final int VELO_RESERVED_020E = 0x020E;
    public static final int VELO_RESERVED_020F = 0x020F;

    public static final int VELO_RESERVED_0210 = 0x0210;
    public static final int VELO_RESERVED_0211 = 0x0211;
    public static final int VELO_RESERVED_0212 = 0x0212;
    public static final int VELO_RESERVED_0213 = 0x0213;
    public static final int VELO_RESERVED_0214 = 0x0214;
    public static final int VELO_RESERVED_0215 = 0x0215;
    public static final int VELO_RESERVED_0216 = 0x0216;
    public static final int VELO_RESERVED_0217 = 0x0217;
    public static final int VELO_RESERVED_0218 = 0x0218;
    public static final int VELO_RESERVED_0219 = 0x0219;
    public static final int VELO_RESERVED_021A = 0x021A;
    public static final int VELO_RESERVED_021B = 0x021B;
    public static final int VELO_RESERVED_021C = 0x021C;
    public static final int VELO_RESERVED_021D = 0x021D;
    public static final int VELO_RESERVED_021E = 0x021E;
    public static final int VELO_RESERVED_021F = 0x021F;

    public static final int VELO_RESERVED_0220 = 0x0220;
    public static final int VELO_RESERVED_0221 = 0x0221;
    public static final int VELO_RESERVED_0222 = 0x0222;
    public static final int VELO_RESERVED_0223 = 0x0223;
    public static final int VELO_RESERVED_0224 = 0x0224;
    public static final int VELO_RESERVED_0225 = 0x0225;
    public static final int VELO_RESERVED_0226 = 0x0226;
    public static final int VELO_RESERVED_0227 = 0x0227;
    public static final int VELO_RESERVED_0228 = 0x0228;
    public static final int VELO_RESERVED_0229 = 0x0229;
    public static final int VELO_RESERVED_022A = 0x022A;
    public static final int VELO_RESERVED_022B = 0x022B;
    public static final int VELO_RESERVED_022C = 0x022C;
    public static final int VELO_RESERVED_022D = 0x022D;
    public static final int VELO_RESERVED_022E = 0x022E;
    public static final int VELO_RESERVED_022F = 0x022F;

    public static final int VELO_RESERVED_0230 = 0x0230;
    public static final int VELO_RESERVED_0231 = 0x0231;
    public static final int VELO_RESERVED_0232 = 0x0232;
    public static final int VELO_RESERVED_0233 = 0x0233;
    public static final int VELO_RESERVED_0234 = 0x0234;
    public static final int VELO_RESERVED_0235 = 0x0235;
    public static final int VELO_RESERVED_0236 = 0x0236;
    public static final int VELO_RESERVED_0237 = 0x0237;
    public static final int VELO_RESERVED_0238 = 0x0238;
    public static final int VELO_RESERVED_0239 = 0x0239;
    public static final int VELO_RESERVED_023A = 0x023A;
    public static final int VELO_RESERVED_023B = 0x023B;
    public static final int VELO_RESERVED_023C = 0x023C;
    public static final int VELO_RESERVED_023D = 0x023D;
    public static final int VELO_RESERVED_023E = 0x023E;
    public static final int VELO_RESERVED_023F = 0x023F;

    public static final int VELO_RESERVED_0240 = 0x0240;
    public static final int VELO_RESERVED_0241 = 0x0241;
    public static final int VELO_RESERVED_0242 = 0x0242;
    public static final int VELO_RESERVED_0243 = 0x0243;
    public static final int VELO_RESERVED_0244 = 0x0244;
    public static final int VELO_RESERVED_0245 = 0x0245;
    public static final int VELO_RESERVED_0246 = 0x0246;
    public static final int VELO_RESERVED_0247 = 0x0247;
    public static final int VELO_RESERVED_0248 = 0x0248;
    public static final int VELO_RESERVED_0249 = 0x0249;
    public static final int VELO_RESERVED_024A = 0x024A;
    public static final int VELO_RESERVED_024B = 0x024B;
    public static final int VELO_RESERVED_024C = 0x024C;
    public static final int VELO_RESERVED_024D = 0x024D;
    public static final int VELO_RESERVED_024E = 0x024E;
    public static final int VELO_RESERVED_024F = 0x024F;

    public static final int VELO_RESERVED_0250 = 0x0250;
    public static final int VELO_RESERVED_0251 = 0x0251;
    public static final int VELO_RESERVED_0252 = 0x0252;
    public static final int VELO_RESERVED_0253 = 0x0253;
    public static final int VELO_RESERVED_0254 = 0x0254;
    public static final int VELO_RESERVED_0255 = 0x0255;
    public static final int VELO_RESERVED_0256 = 0x0256;
    public static final int VELO_RESERVED_0257 = 0x0257;
    public static final int VELO_RESERVED_0258 = 0x0258;
    public static final int VELO_RESERVED_0259 = 0x0259;
    public static final int VELO_RESERVED_025A = 0x025A;
    public static final int VELO_RESERVED_025B = 0x025B;
    public static final int VELO_RESERVED_025C = 0x025C;
    public static final int VELO_RESERVED_025D = 0x025D;
    public static final int VELO_RESERVED_025E = 0x025E;
    public static final int VELO_RESERVED_025F = 0x025F;

    public static final int VELO_RESERVED_0260 = 0x0260;
    public static final int VELO_RESERVED_0261 = 0x0261;
    public static final int VELO_RESERVED_0262 = 0x0262;
    public static final int VELO_RESERVED_0263 = 0x0263;
    public static final int VELO_RESERVED_0264 = 0x0264;
    public static final int VELO_RESERVED_0265 = 0x0265;
    public static final int VELO_RESERVED_0266 = 0x0266;
    public static final int VELO_RESERVED_0267 = 0x0267;
    public static final int VELO_RESERVED_0268 = 0x0268;
    public static final int VELO_RESERVED_0269 = 0x0269;
    public static final int VELO_RESERVED_026A = 0x026A;
    public static final int VELO_RESERVED_026B = 0x026B;
    public static final int VELO_RESERVED_026C = 0x026C;
    public static final int VELO_RESERVED_026D = 0x026D;
    public static final int VELO_RESERVED_026E = 0x026E;
    public static final int VELO_RESERVED_026F = 0x026F;

    public static final int VELO_RESERVED_0270 = 0x0270;
    public static final int VELO_RESERVED_0271 = 0x0271;
    public static final int VELO_RESERVED_0272 = 0x0272;
    public static final int VELO_RESERVED_0273 = 0x0273;
    public static final int VELO_RESERVED_0274 = 0x0274;
    public static final int VELO_RESERVED_0275 = 0x0275;
    public static final int VELO_RESERVED_0276 = 0x0276;
    public static final int VELO_RESERVED_0277 = 0x0277;
    public static final int VELO_RESERVED_0278 = 0x0278;
    public static final int VELO_RESERVED_0279 = 0x0279;
    public static final int VELO_RESERVED_027A = 0x027A;
    public static final int VELO_RESERVED_027B = 0x027B;
    public static final int VELO_RESERVED_027C = 0x027C;
    public static final int VELO_RESERVED_027D = 0x027D;
    public static final int VELO_RESERVED_027E = 0x027E;
    public static final int VELO_RESERVED_027F = 0x027F;

    public static final int VELO_RESERVED_0280 = 0x0280;
    public static final int VELO_RESERVED_0281 = 0x0281;
    public static final int VELO_RESERVED_0282 = 0x0282;
    public static final int VELO_RESERVED_0283 = 0x0283;
    public static final int VELO_RESERVED_0284 = 0x0284;
    public static final int VELO_RESERVED_0285 = 0x0285;
    public static final int VELO_RESERVED_0286 = 0x0286;
    public static final int VELO_RESERVED_0287 = 0x0287;
    public static final int VELO_RESERVED_0288 = 0x0288;
    public static final int VELO_RESERVED_0289 = 0x0289;
    public static final int VELO_RESERVED_028A = 0x028A;
    public static final int VELO_RESERVED_028B = 0x028B;
    public static final int VELO_RESERVED_028C = 0x028C;
    public static final int VELO_RESERVED_028D = 0x028D;
    public static final int VELO_RESERVED_028E = 0x028E;
    public static final int VELO_RESERVED_028F = 0x028F;

    public static final int VELO_RESERVED_0290 = 0x0290;
    public static final int VELO_RESERVED_0291 = 0x0291;
    public static final int VELO_RESERVED_0292 = 0x0292;
    public static final int VELO_RESERVED_0293 = 0x0293;
    public static final int VELO_RESERVED_0294 = 0x0294;
    public static final int VELO_RESERVED_0295 = 0x0295;
    public static final int VELO_RESERVED_0296 = 0x0296;
    public static final int VELO_RESERVED_0297 = 0x0297;
    public static final int VELO_RESERVED_0298 = 0x0298;
    public static final int VELO_RESERVED_0299 = 0x0299;
    public static final int VELO_RESERVED_029A = 0x029A;
    public static final int VELO_RESERVED_029B = 0x029B;
    public static final int VELO_RESERVED_029C = 0x029C;
    public static final int VELO_RESERVED_029D = 0x029D;
    public static final int VELO_RESERVED_029E = 0x029E;
    public static final int VELO_RESERVED_029F = 0x029F;

    public static final int VELO_RESERVED_02A0 = 0x02A0;
    public static final int VELO_RESERVED_02A1 = 0x02A1;
    public static final int VELO_RESERVED_02A2 = 0x02A2;
    public static final int VELO_RESERVED_02A3 = 0x02A3;
    public static final int VELO_RESERVED_02A4 = 0x02A4;
    public static final int VELO_RESERVED_02A5 = 0x02A5;
    public static final int VELO_RESERVED_02A6 = 0x02A6;
    public static final int VELO_RESERVED_02A7 = 0x02A7;
    public static final int VELO_RESERVED_02A8 = 0x02A8;
    public static final int VELO_RESERVED_02A9 = 0x02A9;
    public static final int VELO_RESERVED_02AA = 0x02AA;
    public static final int VELO_RESERVED_02AB = 0x02AB;
    public static final int VELO_RESERVED_02AC = 0x02AC;
    public static final int VELO_RESERVED_02AD = 0x02AD;
    public static final int VELO_RESERVED_02AE = 0x02AE;
    public static final int VELO_RESERVED_02AF = 0x02AF;

    public static final int VELO_RESERVED_02B0 = 0x02B0;
    public static final int VELO_RESERVED_02B1 = 0x02B1;
    public static final int VELO_RESERVED_02B2 = 0x02B2;
    public static final int VELO_RESERVED_02B3 = 0x02B3;
    public static final int VELO_RESERVED_02B4 = 0x02B4;
    public static final int VELO_RESERVED_02B5 = 0x02B5;
    public static final int VELO_RESERVED_02B6 = 0x02B6;
    public static final int VELO_RESERVED_02B7 = 0x02B7;
    public static final int VELO_RESERVED_02B8 = 0x02B8;
    public static final int VELO_RESERVED_02B9 = 0x02B9;
    public static final int VELO_RESERVED_02BA = 0x02BA;
    public static final int VELO_RESERVED_02BB = 0x02BB;
    public static final int VELO_RESERVED_02BC = 0x02BC;
    public static final int VELO_RESERVED_02BD = 0x02BD;
    public static final int VELO_RESERVED_02BE = 0x02BE;
    public static final int VELO_RESERVED_02BF = 0x02BF;

    public static final int VELO_RESERVED_02C0 = 0x02C0;
    public static final int VELO_RESERVED_02C1 = 0x02C1;
    public static final int VELO_RESERVED_02C2 = 0x02C2;
    public static final int VELO_RESERVED_02C3 = 0x02C3;
    public static final int VELO_RESERVED_02C4 = 0x02C4;
    public static final int VELO_RESERVED_02C5 = 0x02C5;
    public static final int VELO_RESERVED_02C6 = 0x02C6;
    public static final int VELO_RESERVED_02C7 = 0x02C7;
    public static final int VELO_RESERVED_02C8 = 0x02C8;
    public static final int VELO_RESERVED_02C9 = 0x02C9;
    public static final int VELO_RESERVED_02CA = 0x02CA;
    public static final int VELO_RESERVED_02CB = 0x02CB;
    public static final int VELO_RESERVED_02CC = 0x02CC;
    public static final int VELO_RESERVED_02CD = 0x02CD;
    public static final int VELO_RESERVED_02CE = 0x02CE;
    public static final int VELO_RESERVED_02CF = 0x02CF;

    public static final int VELO_RESERVED_02D0 = 0x02D0;
    public static final int VELO_RESERVED_02D1 = 0x02D1;
    public static final int VELO_RESERVED_02D2 = 0x02D2;
    public static final int VELO_RESERVED_02D3 = 0x02D3;
    public static final int VELO_RESERVED_02D4 = 0x02D4;
    public static final int VELO_RESERVED_02D5 = 0x02D5;
    public static final int VELO_RESERVED_02D6 = 0x02D6;
    public static final int VELO_RESERVED_02D7 = 0x02D7;
    public static final int VELO_RESERVED_02D8 = 0x02D8;
    public static final int VELO_RESERVED_02D9 = 0x02D9;
    public static final int VELO_RESERVED_02DA = 0x02DA;
    public static final int VELO_RESERVED_02DB = 0x02DB;
    public static final int VELO_RESERVED_02DC = 0x02DC;
    public static final int VELO_RESERVED_02DD = 0x02DD;
    public static final int VELO_RESERVED_02DE = 0x02DE;
    public static final int VELO_RESERVED_02DF = 0x02DF;

    public static final int VELO_RESERVED_02E0 = 0x02E0;
    public static final int VELO_RESERVED_02E1 = 0x02E1;
    public static final int VELO_RESERVED_02E2 = 0x02E2;
    public static final int VELO_RESERVED_02E3 = 0x02E3;
    public static final int VELO_RESERVED_02E4 = 0x02E4;
    public static final int VELO_RESERVED_02E5 = 0x02E5;
    public static final int VELO_RESERVED_02E6 = 0x02E6;
    public static final int VELO_RESERVED_02E7 = 0x02E7;
    public static final int VELO_RESERVED_02E8 = 0x02E8;
    public static final int VELO_RESERVED_02E9 = 0x02E9;
    public static final int VELO_RESERVED_02EA = 0x02EA;
    public static final int VELO_RESERVED_02EB = 0x02EB;
    public static final int VELO_RESERVED_02EC = 0x02EC;
    public static final int VELO_RESERVED_02ED = 0x02ED;
    public static final int VELO_RESERVED_02EE = 0x02EE;
    public static final int VELO_RESERVED_02EF = 0x02EF;

    public static final int VELO_RESERVED_02F0 = 0x02F0;
    public static final int VELO_RESERVED_02F1 = 0x02F1;
    public static final int VELO_RESERVED_02F2 = 0x02F2;
    public static final int VELO_RESERVED_02F3 = 0x02F3;
    public static final int VELO_RESERVED_02F4 = 0x02F4;
    public static final int VELO_RESERVED_02F5 = 0x02F5;
    public static final int VELO_RESERVED_02F6 = 0x02F6;
    public static final int VELO_RESERVED_02F7 = 0x02F7;
    public static final int VELO_RESERVED_02F8 = 0x02F8;
    public static final int VELO_RESERVED_02F9 = 0x02F9;
    public static final int VELO_RESERVED_02FA = 0x02FA;
    public static final int VELO_RESERVED_02FB = 0x02FB;
    public static final int VELO_RESERVED_02FC = 0x02FC;
    public static final int VELO_RESERVED_02FD = 0x02FD;
    public static final int VELO_RESERVED_02FE = 0x02FE;
    public static final int VELO_RESERVED_02FF = 0x02FF;

    public static final int VELO_RESERVED_0300 = 0x0300;
    public static final int VELO_RESERVED_0301 = 0x0301;
    public static final int VELO_RESERVED_0302 = 0x0302;
    public static final int VELO_RESERVED_0303 = 0x0303;
    public static final int VELO_RESERVED_0304 = 0x0304;
    public static final int VELO_RESERVED_0305 = 0x0305;
    public static final int VELO_RESERVED_0306 = 0x0306;
    public static final int VELO_RESERVED_0307 = 0x0307;
    public static final int VELO_RESERVED_0308 = 0x0308;
    public static final int VELO_RESERVED_0309 = 0x0309;
    public static final int VELO_RESERVED_030A = 0x030A;
    public static final int VELO_RESERVED_030B = 0x030B;
    public static final int VELO_RESERVED_030C = 0x030C;
    public static final int VELO_RESERVED_030D = 0x030D;
    public static final int VELO_RESERVED_030E = 0x030E;
    public static final int VELO_RESERVED_030F = 0x030F;

    public static final int VELO_RESERVED_0310 = 0x0310;
    public static final int VELO_RESERVED_0311 = 0x0311;
    public static final int VELO_RESERVED_0312 = 0x0312;
    public static final int VELO_RESERVED_0313 = 0x0313;
    public static final int VELO_RESERVED_0314 = 0x0314;
    public static final int VELO_RESERVED_0315 = 0x0315;
    public static final int VELO_RESERVED_0316 = 0x0316;
    public static final int VELO_RESERVED_0317 = 0x0317;
    public static final int VELO_RESERVED_0318 = 0x0318;
    public static final int VELO_RESERVED_0319 = 0x0319;
    public static final int VELO_RESERVED_031A = 0x031A;
    public static final int VELO_RESERVED_031B = 0x031B;
    public static final int VELO_RESERVED_031C = 0x031C;
    public static final int VELO_RESERVED_031D = 0x031D;
    public static final int VELO_RESERVED_031E = 0x031E;
    public static final int VELO_RESERVED_031F = 0x031F;

    public static final int VELO_RESERVED_0320 = 0x0320;
    public static final int VELO_RESERVED_0321 = 0x0321;
    public static final int VELO_RESERVED_0322 = 0x0322;
    public static final int VELO_RESERVED_0323 = 0x0323;
    public static final int VELO_RESERVED_0324 = 0x0324;
    public static final int VELO_RESERVED_0325 = 0x0325;
    public static final int VELO_RESERVED_0326 = 0x0326;
    public static final int VELO_RESERVED_0327 = 0x0327;
    public static final int VELO_RESERVED_0328 = 0x0328;
    public static final int VELO_RESERVED_0329 = 0x0329;
    public static final int VELO_RESERVED_032A = 0x032A;
    public static final int VELO_RESERVED_032B = 0x032B;
    public static final int VELO_RESERVED_032C = 0x032C;
    public static final int VELO_RESERVED_032D = 0x032D;
    public static final int VELO_RESERVED_032E = 0x032E;
    public static final int VELO_RESERVED_032F = 0x032F;

    public static final int VELO_RESERVED_0330 = 0x0330;
    public static final int VELO_RESERVED_0331 = 0x0331;
    public static final int VELO_RESERVED_0332 = 0x0332;
    public static final int VELO_RESERVED_0333 = 0x0333;
    public static final int VELO_RESERVED_0334 = 0x0334;
    public static final int VELO_RESERVED_0335 = 0x0335;
    public static final int VELO_RESERVED_0336 = 0x0336;
    public static final int VELO_RESERVED_0337 = 0x0337;
    public static final int VELO_RESERVED_0338 = 0x0338;
    public static final int VELO_RESERVED_0339 = 0x0339;
    public static final int VELO_RESERVED_033A = 0x033A;
    public static final int VELO_RESERVED_033B = 0x033B;
    public static final int VELO_RESERVED_033C = 0x033C;
    public static final int VELO_RESERVED_033D = 0x033D;
    public static final int VELO_RESERVED_033E = 0x033E;
    public static final int VELO_RESERVED_033F = 0x033F;

    public static final int VELO_RESERVED_0340 = 0x0340;
    public static final int VELO_RESERVED_0341 = 0x0341;
    public static final int VELO_RESERVED_0342 = 0x0342;
    public static final int VELO_RESERVED_0343 = 0x0343;
    public static final int VELO_RESERVED_0344 = 0x0344;
    public static final int VELO_RESERVED_0345 = 0x0345;
    public static final int VELO_RESERVED_0346 = 0x0346;
    public static final int VELO_RESERVED_0347 = 0x0347;
    public static final int VELO_RESERVED_0348 = 0x0348;
    public static final int VELO_RESERVED_0349 = 0x0349;
    public static final int VELO_RESERVED_034A = 0x034A;
    public static final int VELO_RESERVED_034B = 0x034B;
    public static final int VELO_RESERVED_034C = 0x034C;
    public static final int VELO_RESERVED_034D = 0x034D;
    public static final int VELO_RESERVED_034E = 0x034E;
    public static final int VELO_RESERVED_034F = 0x034F;

    public static final int VELO_RESERVED_0350 = 0x0350;
    public static final int VELO_RESERVED_0351 = 0x0351;
    public static final int VELO_RESERVED_0352 = 0x0352;
    public static final int VELO_RESERVED_0353 = 0x0353;
    public static final int VELO_RESERVED_0354 = 0x0354;
    public static final int VELO_RESERVED_0355 = 0x0355;
    public static final int VELO_RESERVED_0356 = 0x0356;
    public static final int VELO_RESERVED_0357 = 0x0357;
    public static final int VELO_RESERVED_0358 = 0x0358;
    public static final int VELO_RESERVED_0359 = 0x0359;
    public static final int VELO_RESERVED_035A = 0x035A;
    public static final int VELO_RESERVED_035B = 0x035B;
    public static final int VELO_RESERVED_035C = 0x035C;
    public static final int VELO_RESERVED_035D = 0x035D;
    public static final int VELO_RESERVED_035E = 0x035E;
    public static final int VELO_RESERVED_035F = 0x035F;

    public static final int VELO_RESERVED_0360 = 0x0360;
    public static final int VELO_RESERVED_0361 = 0x0361;
    public static final int VELO_RESERVED_0362 = 0x0362;
    public static final int VELO_RESERVED_0363 = 0x0363;
    public static final int VELO_RESERVED_0364 = 0x0364;
    public static final int VELO_RESERVED_0365 = 0x0365;
    public static final int VELO_RESERVED_0366 = 0x0366;
    public static final int VELO_RESERVED_0367 = 0x0367;
    public static final int VELO_RESERVED_0368 = 0x0368;
    public static final int VELO_RESERVED_0369 = 0x0369;
    public static final int VELO_RESERVED_036A = 0x036A;
    public static final int VELO_RESERVED_036B = 0x036B;
    public static final int VELO_RESERVED_036C = 0x036C;
    public static final int VELO_RESERVED_036D = 0x036D;
    public static final int VELO_RESERVED_036E = 0x036E;
    public static final int VELO_RESERVED_036F = 0x036F;

    public static final int VELO_RESERVED_0370 = 0x0370;
    public static final int VELO_RESERVED_0371 = 0x0371;
    public static final int VELO_RESERVED_0372 = 0x0372;
    public static final int VELO_RESERVED_0373 = 0x0373;
    public static final int VELO_RESERVED_0374 = 0x0374;
    public static final int VELO_RESERVED_0375 = 0x0375;
    public static final int VELO_RESERVED_0376 = 0x0376;
    public static final int VELO_RESERVED_0377 = 0x0377;
    public static final int VELO_RESERVED_0378 = 0x0378;
    public static final int VELO_RESERVED_0379 = 0x0379;
    public static final int VELO_RESERVED_037A = 0x037A;
    public static final int VELO_RESERVED_037B = 0x037B;
    public static final int VELO_RESERVED_037C = 0x037C;
    public static final int VELO_RESERVED_037D = 0x037D;
    public static final int VELO_RESERVED_037E = 0x037E;
    public static final int VELO_RESERVED_037F = 0x037F;

    public static final int VELO_RESERVED_0380 = 0x0380;
    public static final int VELO_RESERVED_0381 = 0x0381;
    public static final int VELO_RESERVED_0382 = 0x0382;
    public static final int VELO_RESERVED_0383 = 0x0383;
    public static final int VELO_RESERVED_0384 = 0x0384;
    public static final int VELO_RESERVED_0385 = 0x0385;
    public static final int VELO_RESERVED_0386 = 0x0386;
    public static final int VELO_RESERVED_0387 = 0x0387;
    public static final int VELO_RESERVED_0388 = 0x0388;
    public static final int VELO_RESERVED_0389 = 0x0389;
    public static final int VELO_RESERVED_038A = 0x038A;
    public static final int VELO_RESERVED_038B = 0x038B;
    public static final int VELO_RESERVED_038C = 0x038C;
    public static final int VELO_RESERVED_038D = 0x038D;
    public static final int VELO_RESERVED_038E = 0x038E;
    public static final int VELO_RESERVED_038F = 0x038F;

    public static final int VELO_RESERVED_0390 = 0x0390;
    public static final int VELO_RESERVED_0391 = 0x0391;
    public static final int VELO_RESERVED_0392 = 0x0392;
    public static final int VELO_RESERVED_0393 = 0x0393;
    public static final int VELO_RESERVED_0394 = 0x0394;
    public static final int VELO_RESERVED_0395 = 0x0395;
    public static final int VELO_RESERVED_0396 = 0x0396;
    public static final int VELO_RESERVED_0397 = 0x0397;
    public static final int VELO_RESERVED_0398 = 0x0398;
    public static final int VELO_RESERVED_0399 = 0x0399;
    public static final int VELO_RESERVED_039A = 0x039A;
    public static final int VELO_RESERVED_039B = 0x039B;
    public static final int VELO_RESERVED_039C = 0x039C;
    public static final int VELO_RESERVED_039D = 0x039D;
    public static final int VELO_RESERVED_039E = 0x039E;
    public static final int VELO_RESERVED_039F = 0x039F;

    public static final int VELO_RESERVED_03A0 = 0x03A0;
    public static final int VELO_RESERVED_03A1 = 0x03A1;
    public static final int VELO_RESERVED_03A2 = 0x03A2;
    public static final int VELO_RESERVED_03A3 = 0x03A3;
    public static final int VELO_RESERVED_03A4 = 0x03A4;
    public static final int VELO_RESERVED_03A5 = 0x03A5;
    public static final int VELO_RESERVED_03A6 = 0x03A6;
    public static final int VELO_RESERVED_03A7 = 0x03A7;
    public static final int VELO_RESERVED_03A8 = 0x03A8;
    public static final int VELO_RESERVED_03A9 = 0x03A9;
    public static final int VELO_RESERVED_03AA = 0x03AA;
    public static final int VELO_RESERVED_03AB = 0x03AB;
    public static final int VELO_RESERVED_03AC = 0x03AC;
    public static final int VELO_RESERVED_03AD = 0x03AD;
    public static final int VELO_RESERVED_03AE = 0x03AE;
    public static final int VELO_RESERVED_03AF = 0x03AF;

    public static final int VELO_RESERVED_03B0 = 0x03B0;
    public static final int VELO_RESERVED_03B1 = 0x03B1;
    public static final int VELO_RESERVED_03B2 = 0x03B2;
    public static final int VELO_RESERVED_03B3 = 0x03B3;
    public static final int VELO_RESERVED_03B4 = 0x03B4;
    public static final int VELO_RESERVED_03B5 = 0x03B5;
    public static final int VELO_RESERVED_03B6 = 0x03B6;
    public static final int VELO_RESERVED_03B7 = 0x03B7;
    public static final int VELO_RESERVED_03B8 = 0x03B8;
    public static final int VELO_RESERVED_03B9 = 0x03B9;
    public static final int VELO_RESERVED_03BA = 0x03BA;
    public static final int VELO_RESERVED_03BB = 0x03BB;
    public static final int VELO_RESERVED_03BC = 0x03BC;
    public static final int VELO_RESERVED_03BD = 0x03BD;
    public static final int VELO_RESERVED_03BE = 0x03BE;
    public static final int VELO_RESERVED_03BF = 0x03BF;

    public static final int VELO_RESERVED_03C0 = 0x03C0;
    public static final int VELO_RESERVED_03C1 = 0x03C1;
    public static final int VELO_RESERVED_03C2 = 0x03C2;
    public static final int VELO_RESERVED_03C3 = 0x03C3;
    public static final int VELO_RESERVED_03C4 = 0x03C4;
    public static final int VELO_RESERVED_03C5 = 0x03C5;
    public static final int VELO_RESERVED_03C6 = 0x03C6;
    public static final int VELO_RESERVED_03C7 = 0x03C7;
    public static final int VELO_RESERVED_03C8 = 0x03C8;
    public static final int VELO_RESERVED_03C9 = 0x03C9;
    public static final int VELO_RESERVED_03CA = 0x03CA;
    public static final int VELO_RESERVED_03CB = 0x03CB;
    public static final int VELO_RESERVED_03CC = 0x03CC;
    public static final int VELO_RESERVED_03CD = 0x03CD;
    public static final int VELO_RESERVED_03CE = 0x03CE;
    public static final int VELO_RESERVED_03CF = 0x03CF;

    public static final int VELO_RESERVED_03D0 = 0x03D0;
    public static final int VELO_RESERVED_03D1 = 0x03D1;
    public static final int VELO_RESERVED_03D2 = 0x03D2;
    public static final int VELO_RESERVED_03D3 = 0x03D3;
    public static final int VELO_RESERVED_03D4 = 0x03D4;
    public static final int VELO_RESERVED_03D5 = 0x03D5;
    public static final int VELO_RESERVED_03D6 = 0x03D6;
    public static final int VELO_RESERVED_03D7 = 0x03D7;
    public static final int VELO_RESERVED_03D8 = 0x03D8;
    public static final int VELO_RESERVED_03D9 = 0x03D9;
    public static final int VELO_RESERVED_03DA = 0x03DA;
    public static final int VELO_RESERVED_03DB = 0x03DB;
    public static final int VELO_RESERVED_03DC = 0x03DC;
    public static final int VELO_RESERVED_03DD = 0x03DD;
    public static final int VELO_RESERVED_03DE = 0x03DE;
    public static final int VELO_RESERVED_03DF = 0x03DF;

    public static final int VELO_RESERVED_03E0 = 0x03E0;
    public static final int VELO_RESERVED_03E1 = 0x03E1;
    public static final int VELO_RESERVED_03E2 = 0x03E2;
    public static final int VELO_RESERVED_03E3 = 0x03E3;
    public static final int VELO_RESERVED_03E4 = 0x03E4;
    public static final int VELO_RESERVED_03E5 = 0x03E5;
    public static final int VELO_RESERVED_03E6 = 0x03E6;
    public static final int VELO_RESERVED_03E7 = 0x03E7;
    public static final int VELO_RESERVED_03E8 = 0x03E8;
    public static final int VELO_RESERVED_03E9 = 0x03E9;
    public static final int VELO_RESERVED_03EA = 0x03EA;
    public static final int VELO_RESERVED_03EB = 0x03EB;
    public static final int VELO_RESERVED_03EC = 0x03EC;
    public static final int VELO_RESERVED_03ED = 0x03ED;
    public static final int VELO_RESERVED_03EE = 0x03EE;
    public static final int VELO_RESERVED_03EF = 0x03EF;

    public static final int VELO_RESERVED_03F0 = 0x03F0;
    public static final int VELO_RESERVED_03F1 = 0x03F1;
    public static final int VELO_RESERVED_03F2 = 0x03F2;
    public static final int VELO_RESERVED_03F3 = 0x03F3;
    public static final int VELO_RESERVED_03F4 = 0x03F4;
    public static final int VELO_RESERVED_03F5 = 0x03F5;
    public static final int VELO_RESERVED_03F6 = 0x03F6;
    public static final int VELO_RESERVED_03F7 = 0x03F7;
    public static final int VELO_RESERVED_03F8 = 0x03F8;
    public static final int VELO_RESERVED_03F9 = 0x03F9;
    public static final int VELO_RESERVED_03FA = 0x03FA;
    public static final int VELO_RESERVED_03FB = 0x03FB;
    public static final int VELO_RESERVED_03FC = 0x03FC;
    public static final int VELO_RESERVED_03FD = 0x03FD;
    public static final int VELO_RESERVED_03FE = 0x03FE;
    public static final int VELO_RESERVED_03FF = 0x03FF;

    /* End of the pre-defined short field codes range. */
    public static final int VELO_RESERVED_END = 0x03FF;

    /* Begin of long UUID field code range */
    public static final int LONG_ID_BEGIN = 0x0400;
    /* End of long UUID field code range */
    public static final int LONG_ID_END = 0xFEFF;

    /* Reserved upper range begin */
    public static final int VELO_RESERVED_UPPER_BEGIN = 0xFF00;
    /* Reserved upper range end */
    public static final int VELO_RESERVED_UPPER_END = 0xFFFE;

    /* Invalid field type */
    public static final int INVALID = 0xFFFF;

    /**
     * Can't construct this class.
     */
    private Field() {
        throw new AssertionError();
    }
}
