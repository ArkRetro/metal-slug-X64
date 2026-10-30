/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b2116.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_b2116(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_b20b8();
  candidate_7ba68();
  candidate_7ba72();
  candidate_9bb0();
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
