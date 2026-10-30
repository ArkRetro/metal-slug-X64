/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b63ba.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_b63ba(void)

{
  ushort uVar1;
  undefined2 uVar2;
  char *pcVar3;
  undefined1 *puVar4;
  undefined4 *unaff_A6;
  undefined1 uVar5;

  if (*(char *)(unaff_A6 + 0x1e) == '\0') {
    DAT_0010c6b1 = 0;
  }
  else {
    DAT_0010c6b0 = 0;
  }
  if (*(char *)(unaff_A6 + 0x1e) == '\0') {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *unaff_A6 = 0xb6404;
  if (*(char *)(unaff_A6 + 0x1e) == '\0') {
    pcVar3 = &DAT_0010c6b1;
  }
  else {
    pcVar3 = &DAT_0010c6b0;
  }
  if (*pcVar3 == '\0') {
    uVar1 = candidate_2836();
    if (uVar1 == 0) {
      uVar2 = 0x80;
    }
    else {
      uVar2 = (undefined2)(0x200 / uVar1);
    }
    *(undefined2 *)((int)unaff_A6 + 0x26) = uVar2;
    candidate_1abc8();
    candidate_427a();
    return;
  }
  if (*(char *)(unaff_A6 + 0x1e) == '\0') {
    puVar4 = &DAT_0010c6b1;
  }
  else {
    puVar4 = &DAT_0010c6b0;
  }
  *puVar4 = 0;
  *(undefined2 *)((int)unaff_A6 + 0x26) = 0x100;
  if (*(char *)(unaff_A6 + 0x1e) == '\0') {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *unaff_A6 = 0xb64a2;
  uVar5 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar5) {
    *unaff_A6 = candidate_b63ba;
  }
  return;
}
