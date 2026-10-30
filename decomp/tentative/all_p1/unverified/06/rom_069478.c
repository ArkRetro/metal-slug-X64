/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x069478.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_69478(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_69582();
  candidate_69596();
  unaff_A6[0x14] = 0;
  candidate_4e76();
  candidate_4256();
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
  *(undefined1 *)((int)unaff_A6 + 0x17) = 0xff;
  if (*(char *)((int)unaff_A6 + 0x5b) < '\0') {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  else {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  }
  candidate_19dfc();
  candidate_68f26();
  candidate_68d32();
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *unaff_A6 = 0x67d64;
  uVar3 = 0;
  candidate_1436a();
  candidate_427a();
  if ((bool)uVar3) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  candidate_688d6();
  return;
}
