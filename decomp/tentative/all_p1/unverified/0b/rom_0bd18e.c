/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0bd18e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_bd18e(void)

{
  byte bVar1;
  undefined4 *unaff_A6;

  bRam0010c713 = '\0';
  uRam0010c70e._1_1_ = (char)((uint)uRam0010d7b6 >> 0x10);
  *unaff_A6 = 0xbd1b8;
  bRam0010c712 = (char)((uint)uRam0010d7b6 >> 0x10) - (char)uRam0010c70e & 3;
  bVar1 = bRam0010c713 + 2;
  bRam0010c713 = bVar1 & 7;
  bRam0010c714 = (bVar1 & 6) * '\x02' + bRam0010c712 & 0xf;
  _uRam0010c70e = uRam0010d7b6;
  return;
}
