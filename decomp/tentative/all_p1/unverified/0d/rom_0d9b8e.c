/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d9b8e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d9b8e(void)

{
  code *pcVar1;
  int iVar2;
  undefined2 uVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;

  candidate_9ea8();
  *(undefined1 *)((int)unaff_A6 + 0x12) = 0xff;
  *(undefined1 *)((int)unaff_A6 + 0x13) = 0xff;
  uVar3 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar3;
  candidate_1b1dc();
  candidate_1ad6e();
  candidate_4256();
  *unaff_A6 = 0xd9be2;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = *(undefined2 *)(unaff_A6[3] + 0x1e);
  candidate_da3a8();
  if (*(char *)(unaff_A6[3] + 0x7b) == -1) {
    candidate_4256();
  }
  candidate_1abc8();
  candidate_427a();
  bVar4 = *(byte *)(unaff_A6[3] + 0x60) < 0x44;
  if (*(byte *)(unaff_A6[3] + 0x60) == 0x44) {
    candidate_da254();
    if (!bVar4) goto LAB_000d9c30;
LAB_000d9c48:
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  else {
LAB_000d9c30:
    bVar4 = *(char *)(unaff_A6[3] + 0x7c) != -1;
    if (*(char *)(unaff_A6[3] + 0x7c) == -1) goto candidate_4188;
    candidate_4c24();
    if (bVar4) goto LAB_000d9c48;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    return;
  }
  *(undefined1 *)(unaff_A6[3] + 0x61) = 0xff;
  candidate_1da40();
  candidate_1da40();
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
