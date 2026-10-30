/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0bd5ca.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_bd5ca(void)

{
  undefined4 *unaff_A6;

  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  candidate_4256();
  DAT_0010c70b = '\0';
  *unaff_A6 = 0xbd606;
  if (DAT_0010c70b != '\0') {
    *unaff_A6 = candidate_bd624;
  }
  candidate_1abc8();
  candidate_427a();
  return;
}
