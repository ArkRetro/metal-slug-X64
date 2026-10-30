/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x056a6a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_56a6a(void)

{
  code *pcVar1;
  int iVar2;
  undefined4 uVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  candidate_9ea8();
  candidate_4256();
  uVar3 = 0x8000;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    uVar3 = 0xffff8000;
  }
  unaff_A6[0xc] = uVar3;
  unaff_A6[0xd] = 0x48000;
  *unaff_A6 = 0x56aac;
  uVar4 = 0;
  func_0x000143e4();
  candidate_1945c();
  if (!(bool)uVar4) {
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
      unaff_A6[0xd] = unaff_A6[0xd] + -0xa000;
    }
    else {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
      if ((uint)((int)-unaff_A6[0xd] >> 1) < 0x14000) goto candidate_4188;
      unaff_A6[0xd] = (int)-unaff_A6[0xd] >> 1;
    }
    candidate_427a();
    return;
  }
candidate_4188:
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
