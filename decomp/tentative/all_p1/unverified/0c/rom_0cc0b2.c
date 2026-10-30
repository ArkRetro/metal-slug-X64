/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cc0b2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cc0b2(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_4256();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  *unaff_A6 = 0xcc0da;
  if (*(char *)(unaff_A6 + 0x18) != '\0') {
    unaff_A6[0xc] = unaff_A6[0xc] + 0x1000;
  }
  candidate_1ac38();
  candidate_427a();
  if (0x17f < *(ushort *)(unaff_A6 + 0xf)) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
