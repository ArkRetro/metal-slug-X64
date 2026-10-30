/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x052128.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_52128(void)

{
  code *pcVar1;
  char cVar2;
  int iVar3;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_9ea8();
  candidate_4256();
  candidate_50fb4();
  *(undefined1 *)((int)unaff_A6 + 0x9a) = 0x3c;
  unaff_A6[0xc] = 0xfffe0000;
  unaff_A6[0xd] = 0x58000;
  *unaff_A6 = 0x5216a;
  cVar2 = *(char *)((int)unaff_A6 + 0x9a) + -1;
  *(char *)((int)unaff_A6 + 0x9a) = cVar2;
  if (cVar2 == '\0') {
    candidate_4126();
    iVar3 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar3 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  *(undefined4 *)((int)unaff_A6 + 0x86) = unaff_A6[0xd];
  unaff_A6[0xd] = unaff_A6[0xd] + -0x5000;
  candidate_1ac38();
  candidate_427a();
  if (*(short *)(unaff_A6 + 0x10) < 0x131) {
    iVar3 = *(int *)((int)unaff_A6 + 0x86);
    if (iVar3 < 0) {
      iVar3 = -iVar3;
    }
    unaff_A6[0xd] = iVar3 >> 1;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
  }
  return;
}
