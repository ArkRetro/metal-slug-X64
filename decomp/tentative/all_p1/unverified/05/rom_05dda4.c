/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05dda4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5dda4(void)

{
  code *pcVar1;
  int iVar2;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;

  candidate_9ea8();
  unaff_A6[0x14] = 0x5e954;
  *(undefined2 *)((int)unaff_A6 + 0x4e) = 0x14;
  candidate_1ad6e();
  candidate_4256();
  *unaff_A6 = 0x5dde0;
  bVar3 = false;
  candidate_409a();
  if (!bVar3) {
    candidate_4192();
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  candidate_1abc8();
  iVar2 = unaff_A6[3];
  *(short *)((int)unaff_A6 + 0x1e) = *(short *)(iVar2 + 0x1e) + -1;
  if (*(char *)(iVar2 + 0x86) == '\x01') {
    *unaff_A6 = candidate_5de22;
  }
  *(undefined1 *)(unaff_A6 + 0x22) = *(undefined1 *)(iVar2 + 0x88);
  *(short *)((int)unaff_A6 + 0x1e) = *(short *)((int)unaff_A6 + 0x1e) + -1;
  candidate_427a();
  if (*(char *)(unaff_A6[3] + 0x86) == '\x03') {
    *unaff_A6 = candidate_5de84;
    return;
  }
  if (*(char *)(unaff_A6[3] + 0x86) == '\x02') {
    *unaff_A6 = candidate_5de98;
    return;
  }
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(undefined1 *)(extraout_A0 + 0x5a) = *(undefined1 *)((int)unaff_A6 + 0x5a);
    *(byte *)(unaff_A6[3] + 0x5f) = *(byte *)(unaff_A6[3] + 0x5f) | 0x80;
    *unaff_A6 = candidate_5de98;
  }
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  bVar3 = false;
  candidate_409a();
  if (!bVar3) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
