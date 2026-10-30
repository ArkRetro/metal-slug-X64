/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0826c2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_826c2(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;

  if ((*(byte *)((int)unaff_A6 + 0x15) & 2) == 0) {
    candidate_4256();
  }
  else {
    candidate_4256();
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfd;
  }
  candidate_1b1dc();
  *unaff_A6 = 0x82702;
  uVar3 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar3) {
    if (*(char *)(unaff_A6 + 0x1f) == '\x01') {
      *unaff_A6 = candidate_82742;
      uVar3 = 0;
    }
    else {
      candidate_82ba4();
      *unaff_A6 = 0x828d8;
      uVar3 = 0;
    }
  }
  candidate_1945c();
  if (!(bool)uVar3) {
    return;
  }
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
