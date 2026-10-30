/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04b97e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4b97e(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  candidate_4256();
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
  *unaff_A6 = 0x4b99a;
  candidate_1abc8();
  *(undefined2 *)(unaff_A6 + 0x26) = *(undefined2 *)(unaff_A6[3] + 0x98);
  if (*(short *)(unaff_A6[3] + 0x98) < 0x25) {
    uVar3 = 0;
    candidate_4256();
    candidate_4d9de();
  }
  else {
    uVar3 = 0;
    candidate_4256();
    candidate_4d9de();
  }
  candidate_4df02();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_4ba16;
    uVar3 = 0;
  }
  candidate_4d74a();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
  }
  if (*(char *)(*(int *)(unaff_A6[3] + 0xc) + 0x9d) == '\x7f') {
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
